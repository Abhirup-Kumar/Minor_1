# huffman-coding
My implementation of Huffman Coding compression algorithm made in C to compress and decompress files.

##aanchal: open this in linux.
## go to src folder and write:

## gcc -O3 *.c  [to compile all the code]

## ./a.out c tex.txt compressed.txt [to compress the file named tex to new file...but can name anyhting to this new file , I named compressed.txt]

## ./a.out d compressed.txt ff.txt  [to decompress the file named tex to new file...but can name anything to this new file , I named ff.txt]

##this was it!

## Reference

	void huff_cod(FILE * in, FILE * out)
		Compresses a file using Huffman Coding algorithm.
	Arguments:
		in: file stream to compress
		out: compressed file stream

	void huff_dec(FILE * in, FILE * out)
		Decodes a byte stream using Huffman Coding algorithm.
	Arguments:
		in: compressed file to decode
		out: original file
