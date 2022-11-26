#include "../include/starter1.hpp"

int main(){
    Image pic("picture/clean_finger.png");
    pic.display_image("clean finger");
    waitKey(0);
    pic.minmax();

    cout<<"cast all pixels values into floating numbers"<<endl;
    pic.int_to_float();
    // minimum and maximum in pixel intensity values
    pic.minmax();

    // create white and black squares and save the result in a new image as a png file
    Image img_square (pic);
    img_square.change_pixels(40,125,100,160,1.); //white square
    img_square.change_pixels(160,185,200,250,0.); //black square
    img_square.display_image("finger with white and black squares");
    img_square.save_image("result/finger_with_squares.png");
    waitKey(0);

    // symmetry transform of the image along the y axis and save as a new png file
    Image y_symmetry (pic);
    y_symmetry = y_symmetry.symmetry_yaxis();
    y_symmetry.display_image("symmetry image along the y axis");
    y_symmetry.save_image("result/symmetry_image_y_axis.png");
    waitKey(0);

    // symmetry transform of the image along the diagonal axis y = x and save as a new png file
    Mat sym1 = pic.symmetry_first_diagonal();
    Image s1(sym1);
    s1.display_image("symmetry image along the diagonal axis y = x");
    s1.save_image("result/symmetry_first_diagonal_axis.png");
    waitKey(0);

    // symmetry transform of the image along the diagonal axis y = -x and save as a new png file
    Mat sym2 = pic.symmetry_second_diagonal();
    Image s2(sym2);
    s2.display_image("symmetry image along the diagonal axis y = -x");
    s2.save_image("result/symmetry_second_diagonal_axis.png");
    waitKey(0);

    return 0;
}
