#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <utility>
#include <queue>
#include <unordered_map>
#include <fstream>
#define N 8
static int huff_counter  = 0;
static std::vector<std::pair<unsigned char, std::string>> HUFFMAN_CODE(160);
extern "C"
{
    #define STB_IMAGE_IMPLEMENTATION
    #include "stb_image.h"
}

struct EntropyCoding{
    std::string strTemp1;
    std::string strTemp;
};
EntropyCoding ac_EntropyCodingStr[N*N];

struct StringMap{
    std::string key;
    std::string value;
};

void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        std::swap(*(str+start), *(str+end));
        start++;
        end--;
    }
}
char* itoas(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;
 
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }
 
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0';
    reverse(str, i);
 
    return str;
}

struct AC_EntropyCoding_MiddleSymbol{
    std::string R_S;
    int temp;
};
AC_EntropyCoding_MiddleSymbol ac_EntropyCoding_MiddleSymbol[N*N];


int AC_Difference(int temp){
    int temp1=0;
    if(temp == 0){
         temp1 = 0;
    }else{
        for(int i=1;i<=10;i++){
            if(abs(temp)<pow(2,i)){
                temp1 = i;
                break;
            }
        }
    }
    return temp1;
}

bool AC_EntropyCoding(int F_[8][8],int &index){
    int SSSS[8][8];
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            SSSS[i][j] = AC_Difference(F_[i][j]);
        }
    }
    
    int count = 0;
    int i,j,t;
    for(i=0,j=1,t=1;t<=N-2;t++){
        //To the bottom left
        for(;i<N && j>=0;i++,j--){
            if(F_[i][j]==0){
                count++;
            }else{
                char countString[N*N];
                itoas(count,countString,10);
                std::string strTemp = "/";
                strTemp = countString + strTemp;
                //cout<<"--"<<strTemp<<"--"<<endl;
                char SSSS_String[N*N];
                itoas(SSSS[i][j],SSSS_String,10);
                strTemp = strTemp + SSSS_String;
                //cout<<"**"<<strTemp<<"**"<<endl;
            
                ac_EntropyCoding_MiddleSymbol[index].R_S = strTemp;
                ac_EntropyCoding_MiddleSymbol[index].temp = F_[i][j];
                index++;
                count = 0;
            }
        }
        if(i>=N&&j<0){
            i--;
            j = j+2;
        }else if(i>=N){
            i--;
            j = j+2;
        }else if(j<0){
            j++;
        }
        
        //Upper right
        for(;i>=0&&j<N;i--,j++){
            if(F_[i][j]==0){
                count++;
            }else{
                char countString[N*N];
                itoas(count,countString,10);
                std::string strTemp = "/";
                strTemp = countString + strTemp;
                //cout<<"--"<<strTemp<<"--"<<endl;
                char SSSS_String[N*N];
                itoas(SSSS[i][j],SSSS_String,10);
                strTemp = strTemp + SSSS_String;
                //cout<<"**"<<strTemp<<"**"<<endl;
                
                //Middle symbol
                ac_EntropyCoding_MiddleSymbol[index].R_S = strTemp;
                ac_EntropyCoding_MiddleSymbol[index].temp = F_[i][j];
                index++;
                count = 0;
            }
        }
        if(i<0&&j>=N){
            j--;
            i = i+2;
        }else if(i<0){
            i++;
        }else if(j>=N){
            j--;
            i = i+2;
        }
    }
    
    for(;i<N&&j>=0;i++,j--){
        if(F_[i][j]==0){
            count++;
        }else{
            char countString[N*N];
            itoas(count,countString,10);
            std::string strTemp = "/";
            strTemp = countString + strTemp;
            //cout<<"--"<<strTemp<<"--"<<endl;
            char SSSS_String[N*N];
            itoas(SSSS[i][j],SSSS_String,10);
            strTemp = strTemp + SSSS_String;
            //cout<<"**"<<strTemp<<"**"<<endl;
            
            //Middle symbol
            ac_EntropyCoding_MiddleSymbol[index].R_S = strTemp;
            ac_EntropyCoding_MiddleSymbol[index].temp = F_[i][j];
            index++;
            count = 0;
        }
    }
    if(i>=N){
        i--;
        j = j+2;
    }
    if(F_[i][j]==0){
        count++;
        ac_EntropyCoding_MiddleSymbol[index].R_S = "0/0(EOB)";
        ac_EntropyCoding_MiddleSymbol[index].temp = INT_MAX;
        index++;
    }
    else{
        char countString[N*N];
        itoas(count,countString,10);
        std::string strTemp = "/";
        strTemp = countString + strTemp;
        //cout<<"--"<<strTemp<<"--"<<endl;
        char SSSS_String[N*N];
        itoas(SSSS[i][j],SSSS_String,10);
        strTemp = strTemp + SSSS_String;
        //cout<<"**"<<strTemp<<"**"<<endl;
        
        //Middle symbol
        ac_EntropyCoding_MiddleSymbol[index].R_S = strTemp+"(EOB)";
        ac_EntropyCoding_MiddleSymbol[index].temp = F_[i][j];
        index++;
    }
    
    return true;
}


long long nextPowerOf2(long long p)
{
    
    if (!(p & (p - 1)))
        return p;
    
    return 0x8000000000000000 >> (__builtin_clzll(p) - 1);
}

struct MinHeapNode {
    unsigned char data;
    int freq;
    MinHeapNode *left, *right;

    MinHeapNode(unsigned char data,int freq)

    {

        left = right = NULL;
        this->data = data;
        this->freq = freq;
    }
};
struct compare {

    bool operator()(MinHeapNode* l, MinHeapNode* r)

    {
        return (l->freq > r->freq);
    }
};
void storecodes(struct MinHeapNode* root, std::string str)
{

    if (!root)
        return;

    if (root->data != '$'){
        HUFFMAN_CODE[huff_counter].first = root->data;
        HUFFMAN_CODE[huff_counter++].second = str;
    }

    storecodes(root->left, str + "0");
    storecodes(root->right, str + "1");
}

void HuffmanCodes(std::vector<unsigned char> &data, std::vector<std::pair<std::string , int>> &freq, int size)
{
    struct MinHeapNode *left, *right, *top;
    std::priority_queue<MinHeapNode*, std::vector<MinHeapNode*>, compare> minHeap;

    for (int i = 0; i < size; ++i){
        minHeap.push(new MinHeapNode(data[i], freq[i].second));
    }
    while (minHeap.size() != 1) {
        left = minHeap.top();
        minHeap.pop();

        right = minHeap.top();
        minHeap.pop();
        top = new MinHeapNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        minHeap.push(top);
    }
    storecodes(minHeap.top(), "");
}

bool load_image(std::vector<unsigned char>& image, const std::string& filename, int& x, int&y)
{
    int n;
    unsigned char* data = stbi_load(filename.c_str(), &x, &y, &n, 4);
    if (data != nullptr)
    {
        image = std::vector<unsigned char>(data, data + x * y * 4);
    }
    stbi_image_free(data);
    return (data != nullptr);
}

int main()
{
    std::string filename;
    std::cout<<"Enter the location of the file : ";
    std::cin>>filename;
    int width, height;
    std::vector<unsigned char> image;
    bool success = load_image(image, filename, width, height);
    if (!success)
    {
        std::cout << "Error loading image\n";
        return 1;
    }

//    std::cout << "Image width = " << width << '\n';
//    std::cout << "Image height = " << height << '\n';

    const size_t RGBA = 4;
    
    std::vector<int>r(height * width);
    std::vector<int>b(height * width);
    std::vector<int>g(height * width);
    std::vector<int>a(height * width);

    size_t ar=0;
    for (int i = 0; i < width; i++)
    {
        for(int j = 0 ; j< height ; j++,ar++)
        {    size_t index = RGBA * (j * width + i);
            r[ar]=(image[index + 0]);
            b[ar]=(image[index + 1]) ;
            g[ar]=(image[index + 2]) ;
            a[ar]=(image[index + 3]) ;
        }
    }
    ar=1;
    int l=0,k=0;
    std::string strng="";
    std::vector<std::pair<std::string , int>> temy(160);
    std::vector<std::pair<std::string , int>> temcb(160);
    std::vector<std::pair<std::string , int>> temcr(160);
    temy[0].first = "0/0(EOB)";
    temcb[0].first = "0/0(EOB)";
    temcr[0].first = "0/0(EOB)";
    temy[0].second = 0;
    temcb[0].second = 0;
    temcr[0].second = 0;
    std::vector<unsigned char> HuffmanCode(160);
    for(int i = 1 ; i< 160 ; i++)
    {
        HuffmanCode[i] = static_cast<char>(i);
        temy[i].first = std::to_string((i%10)+1 > 10 ? (i/10)+1 : i/10 ) + "/" + std::to_string((i%10)+1);
        temy[i].second = 0;
        temcr[i].first = std::to_string((i%10)+1 > 10 ? (i/10)+1 : i/10 ) + "/" + std::to_string((i%10)+1);
        temcr[i].second = 0;
        temcb[i].first = std::to_string((i%10)+1 > 10 ? (i/10)+1 : i/10 ) + "/" + std::to_string((i%10)+1);
        temcb[i].second = 0;

    }
    std::vector<std::string> entro_y;
    std::vector<std::string> entro_cb;
    std::vector<std::string> entro_cr;
    while(!(width==l || height==k))
    {
            std::vector<std::vector<int>>mcu_r(width,std::vector<int>(height, 0));
            std::vector<std::vector<int>>mcu_b(width,std::vector<int>(height, 0));
            std::vector<std::vector<int>>mcu_g(width,std::vector<int>(height, 0));
            std::vector<std::vector<float>>mcu_y(width,std::vector<float>(height, 0));
            std::vector<std::vector<float>>mcu_cb(width,std::vector<float>(height, 0));
            std::vector<std::vector<float>>mcu_cr(width,std::vector<float>(height, 0));
            for(int i=0;i<8;i++)
            {
                for(int j=0;j<8;j++)
                {
                    mcu_r[i][j]=r[(l+i)*10+(k+j)];
                    mcu_b[i][j]=r[(l+i)*10+(k+j)];
                    mcu_g[i][j]=r[(l+i)*10+(k+j)];
                }
            }
            for(int i=0;i<8;i++)
            {
                for(int j=0;j<8;j++)
                {
                    mcu_y[i][j]=(int)(0.299*mcu_r[i][j]+0.587*mcu_g[i][j]+0.114*mcu_b[i][j]);
                    mcu_cb[i][j]=(int)(128+(-0.169*mcu_r[i][j]-0.332*mcu_g[i][j]+0.5*mcu_b[i][j]));
                    mcu_cr[i][j]=(int)(128+0.5*mcu_r[i][j]-0.419*mcu_g[i][j]-0.081*mcu_b[i][j]);
                    mcu_y[i][j]-=128;
                    mcu_cb[i][j]-=128;
                    mcu_cr[i][j]-=128;
                }
            }
            const double PI = acos(-1);
            //DCT
            for(int u=0;u<8;u++){
                    for(int v=0;v<8;v++){
                        double tempy = 0.0;
                        double tempcb = 0.0;
                        double tempcr = 0.0;
                        for(int i=0;i<8;i++){
                            for(int j=0;j<8;j++){
                                tempy = tempy + mcu_y[i][j]*cos((2*i+1)*u*PI*1.0/16)*cos((2*j+1)*v*PI*1.0/16);
                                tempcb = tempcb + mcu_cb[i][j]*cos((2*i+1)*u*PI*1.0/16)*cos((2*j+1)*v*PI*1.0/16);
                                tempcr = tempcr + mcu_cr[i][j]*cos((2*i+1)*u*PI*1.0/16)*cos((2*j+1)*v*PI*1.0/16);
                            }
                        }
                        mcu_y[u][v] = 1.0/4*(u==0?1.0/sqrt(2):1)*(v==0?1.0/sqrt(2):1)*tempy;
                        mcu_cb[u][v] = 1.0/4*(u==0?1.0/sqrt(2):1)*(v==0?1.0/sqrt(2):1)*tempcb;
                        mcu_cr[u][v] = 1.0/4*(u==0?1.0/sqrt(2):1)*(v==0?1.0/sqrt(2):1)*tempcr;
                    }
                }
                
            
            l+=8;
            if(l>=width)
            {
                k+=8;
                l=0;
            }
        
            int Q[8][8]={16,11,10,16,24,40,51,61,
                         12,12,14,19,26,58,60,55,
                         14,13,16,24,40,57,69,56,
                         14,17,22,29,51,87,80,62,
                         18,22,37,56,68,109,103,77,
                         24,35,55,64,81,104,113,92,
                         49,64,78,87,103,121,120,101,
                         72,92,95,98,112,100,103,99};
            int C[8][8]={17,18,24,47,99,99,99,99,
                         12,12,14,19,99,99,99,99,
                         14,13,16,99,99,99,99,99,
                         14,17,99,99,99,99,99,99,
                         99,99,99,99,99,99,99,99,
                         99,99,99,99,99,99,99,99,
                         99,99,99,99,99,99,99,99,
                         99,99,99,99,99,99,99,99};
            
            int F_y[8][8];
            int F_cb[8][8];
            
            int F_cr[8][8];
            for(int i=0;i<8;i++){
                for(int j=0;j<8;j++){
                    F_y[i][j] = (int)((mcu_y[i][j]/Q[i][j])>0.0)?floor((mcu_y[i][j]/Q[i][j]) + 0.5): ceil((mcu_y[i][j]/Q[i][j])-0.5);//Quantize, then round
                    F_cb[i][j] = (int)((mcu_cb[i][j]/C[i][j])>0.0)?floor((mcu_cb[i][j]/C[i][j]) + 0.5): ceil((mcu_cb[i][j]/C[i][j])-0.5);//Quantize, then round
                    F_cr[i][j] = (int)((mcu_cr[i][j]/C[i][j])>0.0)?floor((mcu_cr[i][j]/C[i][j]) + 0.5): ceil((mcu_cr[i][j]/C[i][j])-0.5);//Quantize, then round
                }
            }
//            int tempy = F_y[0][0];
//            int tempcb = F_y[0][0];
//            int tempcr = F_y[0][0];
            int indexy=0;
                std::string io = "";
            AC_EntropyCoding(F_y,indexy);
        
             // strng+=std::to_string((int)nextPowerOf2(tempy))+" "+std::to_string(tempy)+"\n";
            for(int p=0;p<indexy;p++){
            //   std::cout<<ac_EntropyCoding_MiddleSymbol[k].R_S<<"\t";////////////////////
               io =ac_EntropyCoding_MiddleSymbol[p].R_S;
                entro_y.push_back(io);
               if(ac_EntropyCoding_MiddleSymbol[p].R_S!="0/0(EOB)"){
                //    std::cout<<ac_EntropyCoding_MiddleSymbol[k].temp<<std::endl;///////////////////////////
                    // io =std::to_string(ac_EntropyCoding_MiddleSymbol[k].temp)+"\n";
                }
                   for (int i = 0; i < 160; i++)
                   {
                       if(temy[i].first == io){
                           ++temy[i].second;
                       }
                   }
            }
            // strng+=std::to_string((int)nextPowerOf2(tempcb))+" "+std::to_string(tempcb)+"\n";
            int indexcb=0;
                io = "";
            AC_EntropyCoding(F_cb,indexcb);
            
            
            for(int p=0;p<indexcb;p++){
            //    std::cout<<ac_EntropyCoding_MiddleSymbol[k].R_S<<"\t";
               io =ac_EntropyCoding_MiddleSymbol[p].R_S;
               entro_cb.push_back(io);
                // if(ac_EntropyCoding_MiddleSymbol[p].R_S!="0/0(EOB)"){
                //    std::cout<<ac_EntropyCoding_MiddleSymbol[k].temp;
                    // io =std::to_string(ac_EntropyCoding_MiddleSymbol[k].temp)+"\n";
                // }
                   for (int i = 0; i < 160; i++)
                   {
                    //    std::cout<<"LOL:- "<<temcb[i].first<<std::endl;
                       if(temcb[i].first == io){
                           ++temcb[i].second;
                       }
                   }
            }
            // strng+=std::to_string((int)nextPowerOf2(tempcr))+" "+std::to_string(tempcr)+"\n";
            int indexcr=0;
                io = "";
            AC_EntropyCoding(F_cr,indexcr);
            for(int p=0;p<indexcr;p++){
            //   std::cout<<ac_EntropyCoding_MiddleSymbol[k].R_S<<"\t";
               io = ac_EntropyCoding_MiddleSymbol[p].R_S;
               entro_cr.push_back(io);
                if(ac_EntropyCoding_MiddleSymbol[p].R_S!="0/0(EOB)"){
                   // std::cout<<ac_EntropyCoding_MiddleSymbol[k].temp;
                    // io = std::to_string(ac_EntropyCoding_MiddleSymbol[k].temp)+"\n";
                }
                   for (int i = 0; i < 160; i++)
                   {
                       if(temcr[i].first == io){
                           ++temcr[i].second;
                       }
                   }
                   
            }
    }
    HuffmanCodes(HuffmanCode, temy, HuffmanCode.size());
    std::vector<std::pair<std::string, std::string>> HUFFMAN_CODE_EXTENDED(160);
    for (int i = 0; i < 160; i++)
    {
        int UT = static_cast<int>(HUFFMAN_CODE[i].first);
        HUFFMAN_CODE_EXTENDED[i].first = std::to_string((UT%10)+1 > 10 ? (UT/10)+1 : UT/10 ) + "/" + std::to_string((UT%10)+1);
        HUFFMAN_CODE_EXTENDED[i].second = HUFFMAN_CODE[i].second;
         //std::cout<<HUFFMAN_CODE_EXTENDED[i].first<<" : "<< HUFFMAN_CODE_EXTENDED[i].second<<std::endl;
    }
    std::ofstream fout;
        fout.open("/Users/ujjwalsrivastava/Downloads/iod/cppNew/cppNew/compress.txt");
        
        if( !fout) {
             std::cerr << "Error: file could not be opened" << std::endl;
             exit(1);
          }
    
    fout << "Huffman Codes are:\n" << std::endl;
        for (size_t i = 0; i < HUFFMAN_CODE_EXTENDED.size()-1; i++) {
            fout << HUFFMAN_CODE_EXTENDED[i].first << " : " << HUFFMAN_CODE_EXTENDED[i].second << std::endl;
        }
        fout << "\nThe encoded string is:" << std::endl;
    
    for (size_t i = 0; i < entro_y.size(); i++)
        for (size_t j = 0; j < HUFFMAN_CODE_EXTENDED.size(); j++)
            if (entro_y[i] == HUFFMAN_CODE_EXTENDED[j].first)
                fout<<HUFFMAN_CODE_EXTENDED[j].second<<" ";
    fout<<std::endl<<std::endl;
    for (size_t i = 0; i < entro_cb.size(); i++)
        for (size_t j = 0; j < HUFFMAN_CODE_EXTENDED.size(); j++)
            if (entro_cb[i] == HUFFMAN_CODE_EXTENDED[j].first)
                fout<<HUFFMAN_CODE_EXTENDED[j].second<<" ";
    fout<<std::endl<<std::endl;
    for (size_t i = 0; i < entro_cr.size(); i++)
        for (size_t j = 0; j < HUFFMAN_CODE_EXTENDED.size(); j++)
            if (entro_cr[i] == HUFFMAN_CODE_EXTENDED[j].first)
                fout<<HUFFMAN_CODE_EXTENDED[j].second<<" ";
    fout.close();
    return 0;
}
