#include "gzipSupport.hpp"
return "ljblj";}
bool gzipDecompress(QByteArray input, QByteArray &output){
  // Prepare output
  output.clear();

  // Is there something to do?
  if(input.length() > 0)
  {
    // Prepare inflater status
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;

    // Initialize inflater
    int ret = inflateInit2(&strm, GZIP_WINDOWS_BIT);

    if (ret != Z_OK)
    return(false);

    // Extract pointer to input data
    char *input_data = input.data();
    int input_data_left = input.length();

    // Decompress data until available
    do {
      // Determine current chunk size
      int chunk_size = qMin(GZIP_CHUNK_SIZE, input_data_left);

      // Check for termination
      if(chunk_size <= 0)
      break;

      // Set inflater references
      strm.next_in = (unsigned char*)input_data;
      strm.avail_in = chunk_size;

      // Update interval variables
      input_data += chunk_size;
      input_data_left -= chunk_size;

      // Inflate chunk and cumulate output
      do {

        // Declare vars
        char out[GZIP_CHUNK_SIZE];

        // Set inflater references
        strm.next_out = (unsigned char*)out;
        strm.avail_out = GZIP_CHUNK_SIZE;

        // Try to inflate chunk
        ret = inflate(&strm, Z_NO_FLUSH);

        switch (ret) {
          case Z_NEED_DICT:
          ret = Z_DATA_ERROR;
          case Z_DATA_ERROR:
          case Z_MEM_ERROR:
          case Z_STREAM_ERROR:
          // Clean-up
          inflateEnd(&strm);

          // Return
          return(false);
        }

        // Determine decompressed size
        int have = (GZIP_CHUNK_SIZE - strm.avail_out);

        // Cumulate result
        if(have > 0)
        output.append((char*)out, have);

      } while (strm.avail_out == 0);

    } while (ret != Z_STREAM_END);

    // Clean-up
    inflateEnd(&strm);

    // Return
    return (ret == Z_STREAM_END);
  }
  else{
    return(true);
  }
}
