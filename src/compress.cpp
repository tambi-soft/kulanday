#include "compress.h"

CompressKpkg::CompressKpkg()
{
    
}

void CompressKpkg::zip(QString in_filename, QString zip_filename)
{
   QFile infile(in_filename);
   QFile outfile(zip_filename);
   infile.open(QIODevice::ReadOnly);
   outfile.open(QIODevice::WriteOnly);
   QByteArray uncompressed_data = infile.readAll();
   QByteArray compressed_data = qCompress(uncompressed_data, 9);
   outfile.write(compressed_data);
   infile.close();
   outfile.close();
}
 
void CompressKpkg::unzip(QString zip_filename, QString out_filename)
{
   QFile infile(zip_filename);
   QFile outfile(out_filename);
   infile.open(QIODevice::ReadOnly);
   outfile.open(QIODevice::WriteOnly);
   QByteArray uncompressed_data = infile.readAll();
   QByteArray compressed_data = qUncompress(uncompressed_data);
   outfile.write(compressed_data);
   infile.close();
   outfile.close();
}

void CompressKpkg::zipDir(QString in_dirname, QString zip_filename)
{
    
}

void CompressKpkg::unzipDir(QString zip_filename, QString out_dirname)
{
    
}
