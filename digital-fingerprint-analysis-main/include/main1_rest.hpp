#ifndef MAIN1_REST_H
#define MAIN1_REST_H
#include "starter1.hpp"

class Ellipse : public Image{
private:
    int a;
    int b;
    Point bary;
public:
    // create an ellipse from a picture of a fingerprint
    Ellipse(Image &image);
    // return the distance between the centre and the top point of the ellipse
    int get_a() const;
    // return the distance between the centre and the far-left point of the ellipse
    int get_b() const;
};

class Mask{
private:
    Image mask;
public:
    // create a mask with a rectangle of r * c pixels in the middle of the image
    Mask(int r,int c,Image &img);
    // create a black ellipse area similar to the fingerprint
    Mask(const Ellipse &s);
    // create a fingerprint ring with a chosen length
    Mask(const Ellipse &s,int length_ring);
    // return the image of the mask
    Image get_mask();
    // return the mask value [i,j]
    int mask_value(int i,int j);
    // return the image after applying the mask
    Image apply_mask_image(const Image &img);
};

class Patch{
protected:
    Image image;
    Mat patch;
    int patch_size;
    Point coordinate;
public:
    // create a random patch from the image with default patch size 9 * 9
    Patch(const Image &img);
    // create a random patch from the image with a patch size (used to build a dictionary)
    Patch(int size,const Image &img);
    // create the surrounding patch in the fingerprint image at coordinates (r,c)
    Patch(int size,int r,int c,const Image &img);
    // return the size of patch
    int get_size();
    // return the matrix of the patch
    Mat get_patch();
    // return the center coordinate of the patch
    Point get_coordinate();
    // return the euclidean distance of intensity between two patches
    float patch_distance(Patch d,Mask m);
    // return the average intensity value of the patch(used for edge detection of fingerprints)
    float average_patch();
};

class Dictionary : public Patch{
private:
    int width;
    int height;
    vector<Point> dictionary;
public:
    // build a dictionary with size rows * cols with a patch size
    Dictionary(int size,int cols,int rows,Image &img);
    // display the dictionary
    void display_dictionary();
    // return a vector storing the distance between the pixel to be replaced and the barycenter (from largest to smallest)
    vector<pair<Point,float>> sort_distance(Mask m);
    // find the closet patch to p in the dictionary
    Patch closest_patch(Patch p,Mask m);
    // restoration of image
    Image copy_paste(Image &image,Mask m);
    // destructor
    ~Dictionary();
};

#endif
