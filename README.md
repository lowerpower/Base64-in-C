# Base64-in-C
Base 64 in C language

Just a simple free to use Base64 implementation in C.

#Encode
```
char *base64_encode(unsigned char const* bytes_to_encode, unsigned int len) 
  Base64_encode encodes the passed byte array of passed length long. Will malloc 
  a destination buffer for return. Caller must free the returned base64 string.
  @param bytes_to_encode - a pointer to the data to be encoded. 
  @param len - then number of bytes to encode
  @return encoded string otherwise NULL
```
#Decode
```
int base64_decode(unsigned char *output, unsigned char *bytes_to_decode)
Input is zero terminated string of base64 encode
```


#License
MIT License
