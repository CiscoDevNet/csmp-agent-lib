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
#include <stdlib.h>

#ifdef OPENSSL
#include <openssl/evp.h>
#include <openssl/ecdsa.h>
#include <openssl/err.h>
#endif

#include "signature_verify.h"

#define SHA256_HASH_SIZE  32

bool signature_verify(const void *data, size_t datalen, const void *sig, size_t siglen)
{
#ifdef OPENSSL
  unsigned char digest[SHA256_HASH_SIZE];
  unsigned int dgst_len = 0;
  int ret;
  EC_KEY *ec_key = NULL;
  EC_GROUP *ec_group;
  unsigned char *pp = (unsigned char*)pubkey;
  EVP_MD_CTX md_ctx;
  unsigned long err_code;
  char err_buf[120];
  ECDSA_SIG *si = NULL;

  // sha256 hash
  EVP_MD_CTX_init(&md_ctx);
  EVP_DigestInit(&md_ctx, EVP_sha256());
  EVP_DigestUpdate(&md_ctx, (const void*)data,datalen);
  EVP_DigestFinal(&md_ctx, digest, &dgst_len);

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
  ec_key = o2i_ECPublicKey(&ec_key,(const unsigned char**)&pp,sizeof(pubkey));
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

#else
  // Temporary stub for first release.
  (void)data;
  (void)datalen;
  (void)sig;
  (void)siglen;
  return true;
#endif
}
