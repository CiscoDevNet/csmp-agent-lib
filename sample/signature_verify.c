/*
 *  Copyright 2021 Cisco Systems, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "osal.h"

#if defined(OPENSSL)
#include <openssl/evp.h>
#include <openssl/ecdsa.h>
#include <openssl/err.h>
#elif defined(MBEDTLS)
#include <stddef.h>
#include <stdbool.h>
#include "mbedtls/pk.h"
#include "mbedtls/sha256.h"
#include "mbedtls/asn1.h"
#endif

#include "signature_verify.h"

#define SHA256_HASH_SIZE  32

#ifdef MBEDTLS
extern void pubkey_get(const char** key, size_t len);
#else
extern void pubkey_get(char *key);
#endif

#ifdef MBEDTLS
/* Parse an ECDSA signature in DER format and extract r and s values. */
static int ecdsa_der_to_rs(const unsigned char *sig, size_t siglen,
                           mbedtls_mpi *r, mbedtls_mpi *s)
{
    int ret;
    const unsigned char *p   = sig;
    const unsigned char *end = sig + siglen;
    size_t len;

    // SEQUENCE
    if ((ret = mbedtls_asn1_get_tag((unsigned char **)&p, end, &len,MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE)) != 0)
    {
        return ret;
    }

    // INTEGER r
    if ((ret = mbedtls_asn1_get_mpi((unsigned char **)&p, end, r)) != 0)
    {
        return ret;
    }

    // INTEGER s
    if ((ret = mbedtls_asn1_get_mpi((unsigned char **)&p, end, s)) != 0)
    {
        return ret;
    }

    return 0;
}

static void ecdsa_cleanup(mbedtls_ecdsa_context *ctx,
                          mbedtls_mpi *r,
                          mbedtls_mpi *s)
{
    if (ctx) mbedtls_ecdsa_free(ctx);
    if (r) mbedtls_mpi_free(r);
    if (s) mbedtls_mpi_free(s);
}
#endif /* MBEDTLS */

bool signature_verify(const void *data, size_t datalen, const void *sig, size_t siglen)
{
#if defined(OPENSSL)
  unsigned char digest[SHA256_HASH_SIZE];
  unsigned int dgst_len = 0;
  int ret;
  EC_KEY *ec_key = NULL;
  EC_GROUP *ec_group;
  char *pp = (char *)osal_malloc(sizeof(char) * PUBLIC_KEY_LEN+1);
  char *pp_o2i = pp;
  pubkey_get(pp);
  EVP_MD_CTX *md_ctx;
  md_ctx = EVP_MD_CTX_new();
  unsigned long err_code;
  ECDSA_SIG *si = NULL;

  // sha256 hash
  EVP_MD_CTX_init(md_ctx);
  EVP_DigestInit(md_ctx, EVP_sha256());
  EVP_DigestUpdate(md_ctx, (const void*)data,datalen);
  EVP_DigestFinal(md_ctx, digest, &dgst_len);

  // creat EC_KEY
  if ((ec_key = EC_KEY_new()) == NULL)
  {
    printf("Error：EC_KEY_new()\n");
    return false;
  }

  // creat curve group
  if ((ec_group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1)) == NULL)
  {
    printf("Error：EC_GROUP_new_by_curve_name()\n");
    EC_KEY_free(ec_key);
    return false;
  }

  // set ec_key group
  ret=EC_KEY_set_group(ec_key,ec_group);
  if(ret!=1)
  {
    printf("Error：EC_KEY_set_group\n");
    EC_KEY_free(ec_key);
    return false;
  }

  // import public key into ec_key
  ec_key = o2i_ECPublicKey(&ec_key,(const unsigned char**)&pp_o2i,PUBLIC_KEY_LEN);
  if (ec_key == NULL)
  {
    printf("Error：o2i_ECPublicKey\n");
    EC_KEY_free(ec_key);
    return false;
  }

  // decode ECSDA_SIG (r, s) from sig
  if (d2i_ECDSA_SIG(&si, (const unsigned char **)&sig, siglen) == NULL) {
    err_code = ERR_peek_last_error();
    printf("d2i_ecdsa error: %s\n", ERR_error_string(err_code, NULL));
  }

  // do verify signature
  ret = ECDSA_do_verify(digest, dgst_len, si, ec_key);

  EC_KEY_free(ec_key);
  ECDSA_SIG_free(si);

  osal_free(pp);
  if(ret==1)
  {
    printf("Signature Verify：OK\n");
    return true;
  }
  else if(ret==0)
  {
    printf("invalid signature\n");
    return false;
  }
  else
  {
    err_code = ERR_peek_last_error();
    printf("verify error: %s\n", ERR_error_string(err_code, NULL));
    return false;
  }

#elif defined(MBEDTLS) // Signature verify function with MbedTLS

  int ret = 0;
  unsigned char hash[SHA256_HASH_SIZE];

  /* Validate input data and datalen */
  if (!data || datalen == 0)
  {
    return false;
  }

  /* Validate input sig and siglen */
  if (!sig || siglen == 0)
  {
    return false;
  }

  const unsigned char *signature = (const unsigned char *)sig;

  /* Hash the message with SHA-256 */
  if ((ret = mbedtls_sha256_ret((const unsigned char *)data, datalen, hash, 0)) != 0)
  {
    return false;
  }

  /* Initialize ECDSA context */
  mbedtls_ecdsa_context ctx;
  mbedtls_ecdsa_init(&ctx);

  /* Load curve group with secp256r1 */
  if ((ret = mbedtls_ecp_group_load(&ctx.grp, MBEDTLS_ECP_DP_SECP256R1)) != 0)
  {
    mbedtls_ecdsa_free(&ctx);
    return false;
  }

  /*Get public key from FND server's SSM certificate for signature verification */
  const char *public_key = NULL;
  pubkey_get(&public_key, PUBLIC_KEY_LEN);
  /* Read public point Q from binary */
  if ((ret = mbedtls_ecp_point_read_binary(&ctx.grp, &ctx.Q, (const unsigned char *)public_key, PUBLIC_KEY_LEN)) != 0)
  {
    mbedtls_ecdsa_free(&ctx);
    return false;
  }

  /* Validate public key against the group */
  if ((ret = mbedtls_ecp_check_pubkey(&ctx.grp, &ctx.Q)) != 0)
  {
      mbedtls_ecdsa_free(&ctx);
      return false;
  }

  /* Parse an ECDSA signature in DER format and extract r and s values. */
  mbedtls_mpi r, s;
  mbedtls_mpi_init(&r);
  mbedtls_mpi_init(&s);
  if (signature[0] == 0x30)
  {
    if ((ret = ecdsa_der_to_rs(signature, siglen, &r, &s)) != 0)
    {
        ecdsa_cleanup(&ctx, &r, &s);
        return false;
    }
  }
  else
  {
      ecdsa_cleanup(&ctx, &r, &s);
      return false;
  }

  /* Do verify signature */
  ret = mbedtls_ecdsa_verify(&ctx.grp, hash, sizeof(hash), &ctx.Q, &r, &s);
  ecdsa_cleanup(&ctx, &r, &s);

  if (ret != 0)
  {
      return false;
  }

  return true;

#else
  // Temporary stub for first release.
  (void)data;
  (void)datalen;
  (void)sig;
  (void)siglen;
  return true;
#endif
}
