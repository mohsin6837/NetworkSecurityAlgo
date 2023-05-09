#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Rotate bits to the left
#define LEFT_ROTATE(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

// SHA-1 constants
#define SHA1_BLOCK_SIZE 64
#define SHA1_DIGEST_SIZE 20

// SHA-1 context structure
typedef struct
{
    uint32_t state[5];
    uint64_t count;
    uint8_t buffer[SHA1_BLOCK_SIZE];
} SHA1_CTX;

// Initialize SHA-1 context
void sha1_init(SHA1_CTX *ctx)
{
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xC3D2E1F0;
    ctx->count = 0;
}
// Perform SHA-1 transformation on a block
void sha1_transform(SHA1_CTX *ctx, const uint8_t *block)
{
    uint32_t a, b, c, d, e, t, i, j, w[80];

    // Prepare the message schedule
    for (i = 0, j = 0; i < 16; ++i, j += 4)
    {
        w[i] = (block[j] << 24) | (block[j + 1] << 16) | (block[j + 2] << 8) | (block[j + 3]);
    }
    for (; i < 80; ++i)
    {
        w[i] = LEFT_ROTATE((w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16]), 1);
    }

    // Initialize hash value for this chunk
    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];
    e = ctx->state[4];

    // Main loop
    for (i = 0; i < 80; ++i)
    {
        if (i < 20)
        {
            t = (d ^ (b & (c ^ d))) + 0x5A827999;
        }
        else if (i < 40)
        {
            t = (b ^ c ^ d) + 0x6ED9EBA1;
        }
        else if (i < 60)
        {
            t = ((b & c) | (d & (b | c))) + 0x8F1BBCDC;
        }
        else
        {
            t = (b ^ c ^ d) + 0xCA62C1D6;
        }

        t += LEFT_ROTATE(a, 5) + e + w[i];
        e = d;
        d = c;
        c = LEFT_ROTATE(b, 30);
        b = a;
        a = t;
    }

    // Update the hash value
    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
}
// Update the SHA-1 context with input data
void sha1_update(SHA1_CTX *ctx, const uint8_t *data, size_t length)
{
    size_t i;
    for (i = 0; i < length; ++i)
    {
        ctx->buffer[(ctx->count % SHA1_BLOCK_SIZE)] = data[i];
        ctx->count++;

        if ((ctx->count % SHA1_BLOCK_SIZE) == 0)
        {
            sha1_transform(ctx, ctx->buffer);
        }
    }
}
// Finish computing the SHA-1 digest
void sha1_final(SHA1_CTX *ctx, uint8_t *digest)
{
    uint64_t bitCount = ctx->count * 8;
    uint8_t padding[SHA1_BLOCK_SIZE];
    size_t paddingSize;
    // Pad the message to ensure a multiple of 512 bits
    memset(padding, 0, SHA1_BLOCK_SIZE);
    padding[0] = 0x80;

    if (ctx->count % SHA1_BLOCK_SIZE < (SHA1_BLOCK_SIZE - 8))
    {
        paddingSize = SHA1_BLOCK_SIZE - 8 - (ctx->count % SHA1_BLOCK_SIZE);
    }
    else
    {
        paddingSize = 2 * SHA1_BLOCK_SIZE - 8 - (ctx->count % SHA1_BLOCK_SIZE);
    }

    sha1_update(ctx, padding, paddingSize);

    // Append the bit count
    ctx->buffer[SHA1_BLOCK_SIZE - 8] = (bitCount >> 56) & 0xFF;
    ctx->buffer[SHA1_BLOCK_SIZE - 7] = (bitCount >> 48) & 0xFF;
    ctx->buffer[SHA1_BLOCK_SIZE - 6] = (bitCount >> 40) & 0xFF;
    ctx->buffer[SHA1_BLOCK_SIZE - 5] = (bitCount >> 32) & 0xFF;
    ctx->buffer[SHA1_BLOCK_SIZE - 4] = (bitCount >> 24) & 0xFF;
    ctx->buffer[SHA1_BLOCK_SIZE - 3] = (bitCount >> 16) & 0xFF;
    ctx->buffer[SHA1_BLOCK_SIZE - 2] = (bitCount >> 8) & 0xFF;
    ctx->buffer[SHA1_BLOCK_SIZE - 1] = bitCount & 0xFF;

    sha1_transform(ctx, ctx->buffer);

    // Output the digest
    for (size_t i = 0; i < SHA1_DIGEST_SIZE; ++i)
    {
        digest[i] = (ctx->state[i >> 2] >> ((3 - (i & 3)) * 8)) & 0xFF;
    }
}
// Calculate the SHA-1 hash of a given input string
void sha1(const uint8_t *data, size_t length, uint8_t *digest)
{
    SHA1_CTX ctx;
    sha1_init(&ctx);
    sha1_update(&ctx, data, length);
    sha1_final(&ctx, digest);
}

int main()
{
    uint8_t digest[SHA1_DIGEST_SIZE];
    const char *message = "Hello, SHA-1!";
    size_t messageLength = strlen(message);
    sha1((const uint8_t *)message, messageLength, digest);
    printf("SHA-1 Digest: ");
    for (size_t i = 0; i < SHA1_DIGEST_SIZE; ++i)
    {
        printf("%02x", digest[i]);
    }
    printf("\n");

    return 0;
}