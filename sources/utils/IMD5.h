//
//  IMD5.hpp
//  tiegao
//
//  Created by mac on 16/5/21.

// const char *text = "hello world";
// IMD5 iMD5;
// iMD5.GenerateMD5(text, strlen(text) );
// const char *result = iMD5.ToString();

//

#ifndef IMD5_hpp
#define IMD5_hpp

#include <string>

using namespace std;

class IMD5
{
private:
#define uint8  unsigned char
#define uint32 unsigned long int
    
    struct md5_context
    {
        uint32 total[2];
        uint32 state[4];
        uint8 buffer[64];
    };
    
    void md5_starts( struct md5_context *ctx );
    void md5_process( struct md5_context *ctx, uint8 data[64] );
    void md5_update( struct md5_context *ctx, uint8 *input, uint32 length );
    void md5_finish( struct md5_context *ctx, uint8 digest[16] );
    
public:
    //! construct a MD5 from any buffer
    void GenerateMD5(unsigned char* buffer,int bufferlen);
    
    //! construct a MD5
    IMD5();
    
    //! construct a md5src from char *
    IMD5(const char * md5src);
    
    //! construct a MD5 from a 16 bytes md5
    IMD5(unsigned long* md5src);
    
    //! add a other md5
    IMD5 operator +(IMD5 adder);
    
    //! just if equal
    bool operator ==(IMD5 cmper);
    
    //! give the value from equer
    // void operator =(IMD5 equer);
    
    //! to a string
    string ToString();
    
    unsigned long m_data[4];
};

#endif /* IMD5_hpp */
