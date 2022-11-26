#include "../include/starter1.hpp"
#include "../include/main1_rest.hpp"

int main(){
    Image pic("picture/weak_finger.png");
    pic.int_to_float();

    // create the dictionary
    int patch_size = 9;
    Dictionary dictionary(patch_size,10,10,pic);
    dictionary.display_dictionary();

    /**************************************************************/
    // Mask (square 30 * 30 in the middle of the fingerprint image)

    Mask square(30,30,pic);
    square.get_mask().save_image("result/square_Mask.png");

    Image mix_square = square.apply_mask_image(pic);
    mix_square.save_image("result/lost_information_fingerprint_square.png");

    Image restoration_square = dictionary.copy_paste(mix_square,square);
    restoration_square.save_image("result/restoration_square.png");


    /**************************************************************/
    // Mask (ring shape mask of fingerprint image)
    Ellipse e(pic);
    Mask ring(e,10);
    ring.get_mask().display_image("Ring Mask");
    ring.get_mask().save_image("result/ring.png");

    Image mix_ring = ring.apply_mask_image(pic);
    mix_ring.display_image("lost information fingerprint ring");
    mix_ring.save_image("result/lost_information_fingerprint_ring.png");

    Image restoration_ring  = dictionary.copy_paste(mix_ring,ring);
    restoration_ring.display_image("Restoration ring");
    restoration_ring.save_image("result/restoration_ring.png");

    return 0;
}