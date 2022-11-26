#include "../include/main1_rest.hpp"

Ellipse::Ellipse(Image &image) : Image(image){
    /* Our purpose is to detect the edge. The idea we are based on is
      that the pixel changes on both sides of the edge we are trying to
      find is huge, like from the white area directly to the black area. */
    bary = image.barycenter();
    // calculate the distance between the far-left point and the center of the ellipse
    for (int i=0;i<bary.x;i++){
        Patch p_col(9,i,bary.y,image);
        float average = p_col.average_patch();
        if (average<=0.9){
            b = bary.x-p_col.get_coordinate().x;
            break;
        }
    }
    // calculate the distance between the top point and the centre of the ellipse
    for (int i=0;i<bary.y;i++){
        Patch p_row(9,bary.x,i,image);
        float average = p_row.average_patch();
        if (average<=0.9){
            a = int(bary.y-p_row.get_coordinate().y);
            break;
        }
    }
}

int Ellipse::get_a()const{
    return a;
}

int Ellipse::get_b()const{
    return b;
}


Mask::Mask(int r,int c,Image &img){
    int height = img.get_height();
    int width = img.get_width();
    int true_false;
    mask = Image(height,width,0);
    // create a rectangle r * c pixels in the middle of the image with True value
    for (int i=int((height-r)/2);i<int((height+r)/2);i++) {
        for (int j=int((width-c)/2);j<int((width+c)/2);j++){
            mask.set_pixel(i,j,1);
        }
    }
}

Mask::Mask(const Ellipse &s){
    // draw an ellipse using barycenter, the long and short axes of the ellipse
    mask = Image(s.get_height(),s.get_width(),1);
    Mat modify = mask.get_matrice();
    Point ss;
    ss.x = s.barycenter().y;
    ss.y = s.barycenter().x;
    ellipse(modify,ss,Size(s.get_a(),s.get_b()),0,0,360,0,-1,0);
    mask = Image(modify);
}

Mask::Mask(const Ellipse &s,int length_ring){
    mask = Image(s.get_height(),s.get_width(),0);
    Mat modify = mask.get_matrice();
    Point ss;
    ss.x = s.barycenter().y;
    ss.y = s.barycenter().x;
    // here we modify the thickness of the line of the ellipse
    // draw a white fingerprint ring with a chosen length
    ellipse(modify,ss,Size(s.get_a(),s.get_b()),0,0,360,1,length_ring,0);
    mask = Image(modify);
}

Image Mask::get_mask(){
    return mask;
}

int Mask::mask_value(int i,int j){
    return mask.get_pixel(i,j);
}


Image Mask::apply_mask_image(const Image &img){
    // We apply the mask values to the original image (used to create the missing information image)
    Image mask_image(img.get_height(),img.get_width(),1);
    for (int i=0;i<img.get_height();i++){
        for (int j=0;j<img.get_width();j++){
            if (mask_value(i,j)==0) {
                mask_image.set_pixel(i, j, img.get_pixel(i, j));
            }
        }
    }
    return mask_image;
}

Patch::Patch(const Image &img){
    image = img;
    patch_size = 9;
    int len = int((patch_size-1)/2);
    // random coordinate in the original image
    int random_i = rand() % (image.get_height()-patch_size);
    int random_j = rand() % (image.get_width()-patch_size);
    patch = Mat::zeros(patch_size,patch_size,CV_32FC1);
    for (int i=0;i<patch_size;i++){
        for (int j=0;j<patch_size;j++){
            patch.at<float>(i,j) = image.get_pixel(random_i+i,random_j+j);
        }
    }
    coordinate.x = random_i+len;
    coordinate.y = random_j+len;
}

Patch::Patch(int size, const Image &img){
    image = img;
    if (size % 2 == 0){
        cout<<"The patch size should be an odd number."<<endl;
        exit(1);
    }else{
        patch_size = size;
    }
    int len = int((patch_size-1)/2);
    // random coordinate in the original image
    int random_i = rand() % (image.get_height()-patch_size);
    int random_j = rand() % (image.get_width()-patch_size);
    patch = Mat::zeros(patch_size,patch_size,CV_32FC1);
    for (int i=0;i<patch_size;i++){
        for (int j=0;j<patch_size;j++){
            patch.at<float>(i,j) = image.get_pixel(random_i+i,random_j+j);
        }
    }
    coordinate.x = random_i+len;
    coordinate.y = random_j+len;
}


Patch::Patch(int size, int r,int c, const Image &img){
    /* Create the surrounding patch in the fingerprint image at coordinates (r,c)
       For the pixels at the edges, we can't take an area around them,
       so we discuss them in four cases: top, bottom, far left and far right. */
    if (size % 2 == 0){
        cout<<"The patch size should be an odd number."<<endl;
        exit(1);
    }else{
        patch_size = size;
    }
    image = img;
    Mat mat_img = img.get_matrice();
    int height = img.get_height();
    int width = img.get_width();
    int len = int((patch_size-1)/2);
    patch = Mat::zeros(patch_size,patch_size,CV_32FC1);
    coordinate.x = r;
    coordinate.y = c;

    // if the pixel is at the top, we default to taking the closet available patch at the top
    if (r<=len) {
        coordinate.x = len;
        if (c>len and c<width-len) {
            patch = mat_img(Range(0,patch_size),Range(c-len,c+len+1));
        }else if (c<=len) {
            patch = mat_img(Range(0,patch_size),Range(0,patch_size));
            coordinate.y = len;
        }else{
            patch = mat_img(Range(0,patch_size),Range(0,patch_size));
            patch = mat_img(Range(0,patch_size),Range(width-patch_size,width));
            coordinate.y = width-len-1;
        }

        // if the pixel is at the bottom, we default to taking the closet available patch at the bottom
    }else if (r>=height-len) {
        coordinate.x = height-len-1;
        if (c>len and c<width-len) {
            patch = mat_img(Range(height-patch_size,height),Range(c-len,c+len+1));
        }else if (c<=len) {
            patch = mat_img(Range(height-patch_size,height),Range(0,patch_size));
            coordinate.y = len;
        }else{
            patch = mat_img(Range(height-patch_size,height),Range(width-patch_size,width));
            coordinate.y = width-len-1;
        }

        // if the pixel is on the far left, we default to taking the closet available patch on the far left
    }else if (c<=len) {
        coordinate.y = len;
        patch = mat_img(Range(r-len,r+len+1),Range(0,patch_size));

        // if the pixel is on the far right, we default to taking the closet available patch on the far right
    }else if (c>=width-len) {
        coordinate.y = width-len-1;
        patch = mat_img(Range(r-len,r+len+1),Range(width-patch_size,width));
    }else{
        patch = mat_img(Range(r-len,r+len+1), Range(c-len,c+len+1));
    }
}

int Patch::get_size(){
    return patch_size;
}

Mat Patch::get_patch(){
    return patch;
}

Point Patch::get_coordinate(){
    return coordinate;
}

float Patch::patch_distance(Patch d,Mask m){
    int len = int((patch_size-1)/2);
    float distance = 0;
    float intensity_patch, intensity_d;
    for (int i=0;i<patch_size;i++){
        for (int j=0;j<patch_size;j++){
            // only the valid pixels of patch used for distance computation
            if (m.mask_value(coordinate.x-len+i,coordinate.y-len+j)==0){
                // calculate the euclidean distance of intensity
                intensity_patch = image.get_pixel(coordinate.x,coordinate.y);
                intensity_d = image.get_pixel(d.coordinate.x,d.coordinate.y);
                distance += pow(intensity_patch-intensity_d,2);
            }
        }
    }
    return distance;
}

float Patch::average_patch(){
    float average=0;
    for (int i=0;i<patch_size;i++){
        for (int j=0;j<patch_size;j++){
            average += patch.at<float>(i,j);
        }
    }
    average = average / pow(patch_size,2);
    return average;
}

Dictionary::Dictionary(int size,int cols,int rows,Image &img): Patch(size,img){
    srand((unsigned int) time(NULL));
    width = cols;
    height = rows;
    for (int i=0;i<height;i++){
        for (int j=0;j<width;j++){
            Patch* p = new Patch(size,img);
            dictionary.push_back(p->get_coordinate());
            delete p;
        }
    }
}

Dictionary::~Dictionary(){
}

void Dictionary::display_dictionary(){
    // display the dictionary with borders
    // The idea is each row consist of width * small squares, then we concatenate each row
    // The code looks long because we add borders around each patch to make the dictionary look better.
    Mat concatenate,line,white_bord_col,black_bord_col,black_bord_row,white_bord_row,next_patch;
    concatenate = Mat::zeros(1,width*(patch_size+3)+1,CV_32FC1);
    for (int i=1;i<width;i++){
        concatenate.at<float>(0,(i*(patch_size+3)-1))=1;
    }
    for (int i=0;i<height;i++){
        line = Mat::zeros(patch_size,1,CV_32FC1);
        for(int j=i*width;j<(i+1)*width;j++){
            Patch next(patch_size,dictionary[j].x,dictionary[j].y,image);
            Mat next_patch = next.get_patch();
            hconcat(line,next_patch,line);
            black_bord_col = Mat::zeros(patch_size,1,CV_32FC1);
            white_bord_col = Mat::ones(patch_size,1,CV_32FC1);
            hconcat(line,black_bord_col,line);
            hconcat(line,white_bord_col,line);
            hconcat(line,black_bord_col,line);
        }
        vconcat(concatenate,line,concatenate);
        black_bord_row = Mat::zeros(1,width*(patch_size+3)+1,CV_32FC1);
        for (int i=1;i<width;i++){
            black_bord_row.at<float>(0,(i*(patch_size+3)-1))=1;
        }
        vconcat(concatenate,black_bord_row,concatenate);
        white_bord_row = Mat::ones(1,width*(patch_size+3)+1,CV_32FC1);
        vconcat(concatenate,white_bord_row,concatenate);
        vconcat(concatenate,black_bord_row,concatenate);
    }
    concatenate = concatenate(Range(0,concatenate.rows-2),Range(0,concatenate.cols-2));

    Image concat(concatenate);
    concat.display_image("dictionary");
    concat.save_image("result/dictionary.png");
}

vector<pair<Point,float>> Dictionary::sort_distance(Mask m){
    vector<pair<Point,float>> distance;
    float d;
    for (int i=0;i<image.get_height();i++){
        for (int j=0;j<image.get_width();j++){
            if (m.mask_value(i,j)==1){
                Point bary = image.barycenter();
                d = image.bary_distance(i,j,bary);
                distance.push_back(pair<Point,float>(Point(i,j),d));
            }
        }
    }

    // Sort the pixels to be replaced according to the distance to the barycenter, from largest to smallest
    int i,j;
    for (i=0;i<distance.size();i++){
        if (distance[i].second > distance[i-1].second){
            pair<Point,float> temp = distance[i];
            for (j=i;j>0;j--){
                if (distance[j-1].second < temp.second){
                    distance[j] = distance[j-1];
                }else{
                    break;
                }
            }
            distance[j] = temp;
        }
    }
    return distance;
}

Patch Dictionary::closest_patch(Patch p,Mask m){
    Patch closest(patch_size,dictionary[0].x,dictionary[0].y,image);
    float min = closest.patch_distance(p,m);
    float d;
    float min_patch_value = 0;
    for (int i=1;i<dictionary.size();i++){
        Patch *next = new Patch(patch_size,dictionary[i].x,dictionary[i].y,image);
        d = next->patch_distance(p,m);
        if (d < min){
            min = d;
            closest = *next;
        }
        delete next;
    }
    return closest;
}

Image Dictionary::copy_paste(Image &image,Mask m){
    cout<<"The program takes minutes to calculate!"<<endl;
    image.int_to_float();
    Image modify(image);
    Point min;
    float value=0;
    vector<pair<Point,float>> sort_coordinate = sort_distance(m);
    for (int i=0;i<sort_coordinate.size();i++){
        Point t = sort_coordinate[i].first;
        Patch *p = new Patch(patch_size, t.x, t.y, modify);
        Patch min_patch = closest_patch(*p, m);
        min = min_patch.get_coordinate();
        value = image.get_pixel(min.x, min.y);
        modify.set_pixel(t.x, t.y, value);
        delete p;
    }
    return modify;
}
