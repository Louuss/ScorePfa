#ifndef GZIPSUPPORT
#define GZIPSUPPORT

#include <zlib.h>
#include <QByteArray>

#define GZIP_WINDOWS_BIT 15 + 16
#define GZIP_CHUNK_SIZE 32 * 1024

bool gzipDecompress(QByteArray input, QByteArray &output);

#endif
