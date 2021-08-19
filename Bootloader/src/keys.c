#include <bootutil/sign_key.h>

#include "keys/key_enc_priv.c.import"
#include "keys/key_sign_pub.c.import"

SECURE_STORAGE const struct bootutil_key bootutil_enc_key = {
	.key = enc_priv_key,
	.len = &enc_priv_key_len,
};

const struct bootutil_key bootutil_keys[] = {
    [0] = {
        .key = ecdsa_pub_key,
        .len = &ecdsa_pub_key_len,
    }
};

const int bootutil_key_cnt = sizeof(bootutil_keys) / sizeof(bootutil_keys[0]);