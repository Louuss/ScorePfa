#ifndef GZIP_SUPPORT
#define GZIP_SUPPORT

#include <QByteArray>

#include <zlib.h>

#define GZIP_WINDOWS_BIT 15 + 16
#define GZIP_CHUNK_SIZE 32 * 1024

/**
  \file gzipSupport.hpp
  \brief Decompress an als file into a xml file

  */

/**
  \fn bool gzipDecompress(QByteArray input, QByteArray &output)
  \brief This function decompresses an als file into a xml file

  \param input An als file previously selected
  \param output The file where is written the decompressed data
  \return True if the conversion succeeded, False otherwise
    */

bool gzipDecompress(QByteArray input, QByteArray& output);

#endif
