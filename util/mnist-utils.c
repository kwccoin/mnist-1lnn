/**
 * @file mnist-utils.c
 * @brief Utitlies for handling the MNIST files
 * @see http://yann.lecun.com/exdb/mnist/
 * @author Matt Lind
 * @date July 2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "mnist-utils.h"




/**
 * @details Reverse byte order in 32bit numbers
 * MNIST files contain all numbers in reversed byte order,
 * and hence must be reversed before using
 */

uint32_t flipBytes(uint32_t n){
    
    uint32_t b0,b1,b2,b3;
    
    b0 = (n & 0x000000ff) <<  24u;
    b1 = (n & 0x0000ff00) <<   8u;
    b2 = (n & 0x00ff0000) >>   8u;
    b3 = (n & 0xff000000) >>  24u;
    
    return (b0 | b1 | b2 | b3);
    
}




/**
 * @details Read MNIST image file header
 * @see http://yann.lecun.com/exdb/mnist/ for definition details
 */

void readImageFileHeader(FILE *imageFile, MNIST_ImageFileHeader *ifh){
    
    ifh->magicNumber =0;
    ifh->maxImages   =0;
    ifh->imgWidth    =0;
    ifh->imgHeight   =0;
    
    FILE *flog;
    flog = fopen("/Users/blue5/Documents/GitHub-blue5/mnist-1lnn/ifhdng_log1.txt", "w+");
    
    fprintf (flog,"ifh->magicNumber before: %08x\n",
            (unsigned int) ifh->magicNumber ); /*|
            (unsigned int) ifh->magicNumber << 8); */
   
    fread(&ifh->magicNumber, 4, 1, imageFile);
    ifh->magicNumber = flipBytes(ifh->magicNumber);
    
    /* the hex is ok i.e. */
    /* 00000000: 00 00 08 03 00 00 27 10 	......'.   magic no: 0x0803         image no: 0x2710 i.e.10k images
     00000008: 00 00 00 1C 00 00 00 1C 	........   width:    0x1C   i.e. 28 height:   0x1C   i.e. 28 */
    /* ifh->magicNumber before: 00000000
     ifh->magicNumber after: 00000803
     
     ifh->maxImages before: 00000000
     ifh->maxImages after: 00002710
     
     ifh->imgWidth before: 00000000
     ifh->imgWidth after: 0000001c
     
     ifh->imgHeight before: 00000000
     ifh->imgHeight after: 0000001c */

    /* the little endian assumption is that if you have a no 0A0B0C0D
        it will stored starting with 0D0C0B0A LSB first
        in memory or in file i.e. receive the least sign. no. first
        -- against network byte order --
        and hence if the file hex is 00 00 27 10 as above
        the number read assume little endian it will be read and becomes 10270000
        strange way to store it but that is how it was stored 
        hence need flip */
    
    fprintf (flog,"ifh->magicNumber after: %08x\n\n",
            (unsigned int) ifh->magicNumber ); /*|
            (unsigned int) ifh->magicNumber << 8); */

    fprintf (flog,"ifh->maxImages before: %08x\n",
            (unsigned int) ifh->maxImages ); /*|
            (unsigned int) ifh->maxImages << 8); */

    fread(&ifh->maxImages, 4, 1, imageFile);
    ifh->maxImages = flipBytes(ifh->maxImages);
    
    fprintf (flog,"ifh->maxImages after: %08x\n\n",
            (unsigned int) ifh->maxImages ); /*|
            (unsigned int) ifh->maxImages << 8); */
    
    fprintf (flog,"ifh->imgWidth before: %08x\n",
            (unsigned int) ifh->imgWidth ); /*|
            (unsigned int) ifh->imgWidth << 8); */
    
   
    fread(&ifh->imgWidth, 4, 1, imageFile);
    ifh->imgWidth = flipBytes(ifh->imgWidth);
    
    fprintf (flog,"ifh->imgWidth after: %08x\n\n",
            (unsigned int) ifh->imgWidth ); /*|
            (unsigned int) ifh->imgWidth << 8); */
    
    fprintf (flog,"ifh->imgHeight before: %08x\n",
            (unsigned int) ifh->imgHeight ); /*|
            (unsigned int) ifh->imgHeight << 8); */
    
    
    fread(&ifh->imgHeight, 4, 1, imageFile);
    ifh->imgHeight = flipBytes(ifh->imgHeight);
    
    fprintf (flog,"ifh->imgHeight after: %08x\n\n",
            (unsigned int) ifh->imgHeight ); /*|
            (unsigned int) ifh->imgHeight << 8); */
    
    fclose(flog);
    
}




/**
 * @details Read MNIST label file header
 * @see http://yann.lecun.com/exdb/mnist/ for definition details
 */

void readLabelFileHeader(FILE *imageFile, MNIST_LabelFileHeader *lfh){
    
    lfh->magicNumber =0;
    lfh->maxImages   =0;
    
    fread(&lfh->magicNumber, 4, 1, imageFile);
    lfh->magicNumber = flipBytes(lfh->magicNumber);
    
    fread(&lfh->maxImages, 4, 1, imageFile);
    lfh->maxImages = flipBytes(lfh->maxImages);
    
}




/**
 * @details Open MNIST image file and read header info
 * by reading the header info, the read pointer
 * is moved to the position of the 1st IMAGE
 */

FILE *openMNISTImageFile(char *fileName){

    FILE *imageFile;
    imageFile = fopen (fileName, "rb");
    if (imageFile == NULL) {
        printf("Abort! Could not fine MNIST IMAGE file: %s\n",fileName);
        exit(0);
    }

    MNIST_ImageFileHeader imageFileHeader;
    readImageFileHeader(imageFile, &imageFileHeader);
    
    return imageFile;
}




/**
 * @details Open MNIST label file and read header info
 * by reading the header info, the read pointer
 * is moved to the position of the 1st LABEL
 */

FILE *openMNISTLabelFile(char *fileName){
    
    FILE *labelFile;
    labelFile = fopen (fileName, "rb");
    if (labelFile == NULL) {
        printf("Abort! Could not find MNIST LABEL file: %s\n",fileName);
        exit(0);
    }

    MNIST_LabelFileHeader labelFileHeader;
    readLabelFileHeader(labelFile, &labelFileHeader);
    
    return labelFile;
}




/**
 * @details Returns the next image in the given MNIST image file
 */

MNIST_Image getImage(FILE *imageFile){
    
    MNIST_Image img;
    size_t result;
    result = fread(&img, sizeof(img), 1, imageFile);
    if (result!=1) {
        printf("\nError when reading IMAGE file! Abort!\n");
        exit(1);
    }
    
    return img;
}




/**
 * @details Returns the next label in the given MNIST label file
 */

MNIST_Label getLabel(FILE *labelFile){
    
    MNIST_Label lbl;
    size_t result;
    result = fread(&lbl, sizeof(lbl), 1, labelFile);
    if (result!=1) {
        printf("\nError when reading LABEL file! Abort!\n");
        exit(1);
    }
    
    return lbl;
}


