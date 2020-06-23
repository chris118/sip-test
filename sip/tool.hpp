//
//  tool.h
//  sip
//
//  Created by Chris on 2018/9/20.
//  Copyright © 2018年 Chris. All rights reserved.
//

#ifndef tool_h
#define tool_h
void printBuffer(char* buff, int size, std::string tag){
    std::cout << tag << std::endl;
    for(int i = 0; i < size; i++){
        printf("%02x", buff[i]);
    }
    std::cout << std::endl;
}
/*两个字符转换成一个字符，长度为原来的1/2*/
static void Hex2Char(char *szHex, unsigned char *rch)
{
    int i;
    for(i=0; i<2; i++)
    {
        if(*(szHex + i) >='0' && *(szHex + i) <= '9')
            *rch = (*rch << 4) + (*(szHex + i) - '0');
        else if(*(szHex + i) >='a' && *(szHex + i) <= 'f')
            *rch = (*rch << 4) + (*(szHex + i) - 'a' + 10);
        else
            break;
    }
}

/*十六进制char* 转 Binary char*函数*/
void HexStr2CharStr( char *pszHexStr, int iSize,  char *pucCharStr)
{
    int i;
    unsigned char ch;
    if (iSize%2 != 0) return;
    for(i=0; i<iSize/2; i++)
    {
        Hex2Char(pszHexStr+2*i, &ch);
        pucCharStr[i] = ch;
    }
}

/*单个字符转十六进制字符串，长度增大2被*/
static void Char2Hex(unsigned char ch, char *szHex)
{
    int i;
    unsigned char byte[2];
    byte[0] = ch/16;
    byte[1] = ch%16;
    for(i=0; i<2; i++)
    {
        if(byte[i] >= 0 && byte[i] <= 9)
            szHex[i] = '0' + byte[i];
        else
            szHex[i] = 'a' + byte[i] - 10;
    }
    szHex[2] = 0;
}

/*字符串转换函数，中间调用上面的函数*/
void CharStr2HexStr( char *pucCharStr, int iSize,  char *pszHexStr)
{
    int i;
    char szHex[3];
    pszHexStr[0] = 0;
    for(i=0; i<iSize; i++)
    {
        Char2Hex(pucCharStr[i], szHex);
        strcat(pszHexStr, szHex);
    }
}


#endif /* tool_h */
