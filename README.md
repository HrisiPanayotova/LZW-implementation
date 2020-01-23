# LZW compression algorithm - implementation

Implementation of the LZW compression algorithm in two variants. The first one - fixed, compresses the files using exactly 3 bytes for each code. The second one - changeable, different compression length codes for each file, as it determines the amount of bits needed for each code. The project uses C++17.

Usage:

 * `-comprff <inputFileName> <outputFileName>` - compresses the input file in the file with name outputFileName using the fixed compression

 * `-comprfc <inputFileName> <outputFileName>` - compresses the input file in the file with name outputFileName using the changeable compression
 
 * `-decomprff <inputFileName> <outputFileName>` - decompresses the input file in the file with name outputFileName using the fixed decompression
 
 * `-decomprfc <inputFileName> <outputFileName>` - decompresses the input file in the file with name outputFileName using the changeable decompression
 
 * `-comprdirall <inputDirName>` - compresses all file in the folder with name `inputDirName`, by creating a 2 folders called `changeableLZWCompression` & `fixedLZWCompression`. All the files in directory will be compressed with the changeable compression in `changeableLZWCompression` and with the fixed -> `fixedLZWCompression`. A csv file `result.csv` will be created, containing information on the compression results.

 * `-comprdirallcantrbry` - calls `-comprdirall <inputDirName>` with the cantrbry folder from the resources. Keep in mind that you have to wait a bit for the compression to end
 
 * `-decomprdirf <inputDirName> <outputDirName>` - decompresses all the files from `inputDirName` to the folder denoted by `outputDirName` using the fixed decompression 

 * `-decomprdirc <inputDirName> <outputDirName>` - decompresses all the files from `inputDirName` to the folder denoted by `outputDirName` using the changeable decompression

