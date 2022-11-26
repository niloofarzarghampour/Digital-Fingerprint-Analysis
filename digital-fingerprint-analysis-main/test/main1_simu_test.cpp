#include "../include/starter1.hpp"
#include "../include/main1_rest.hpp"

int main(){
    // display the barycenter of the image
    Image pic1("picture/weak_finger.png");
    pic1.int_to_float();
    Point bary = pic1.barycenter();
    pic1.change_pixels(bary.y-5,bary.y+5,bary.x-5,bary.x+5,1);
    pic1.display_image("barycenter");
    pic1.save_image("result/barycenter.png");

    /***************************************************/

    Image pic("picture/clean_finger.png");
    pic.int_to_float();

    // Test for isotropic
    // c(r) = exp(-r^2 / 1000)
    Image pic2(pic);
    pic2.isotropic(1);
    pic2.display_image("isotropic c(r)=exp(-r^2/1000)");
    pic2.save_image("result/isotropic1.png");

    // c(r) = 1/(r^2/10000 + 1)
    Image pic3(pic);
    pic3.isotropic(2);
    pic3.display_image("isotropic c(r)=1/(r^2/1000+1)");
    pic3.save_image("result/isotropic2.png");

    // c(r) = 1/(1+exp^(0.45*r-30))
    Image pic4(pic);
    pic4.isotropic(3);
    pic4.display_image("isotropic c(r)=1/(1+exp^(0.45*r-30))");
    pic4.save_image("result/isotropic3.png");

    // Test for finding the ellipse of fingerprint zone
    Ellipse e(pic);
    Mask finger(e);
    finger.get_mask().display_image("Fingerprint zone");
    finger.get_mask().save_image("result/finger_zone.png");

    // Test for anisotropic
    int angle = 85;
    Image pic5(pic);
    pic5 = pic5.anisotropic(1,angle);
    pic5.display_image("anisotropic c(r)=exp(-r^2/10000)");
    pic5.save_image("result/anisotropic1.png");

    // c(r) = 1/(r^2/10000 + 1)
    Image pic6(pic);
    pic6 = pic6.anisotropic(2,angle);
    pic6.display_image("anisotropic c(r)=1/(r^2/10000+1)");
    pic6.save_image("result/anisotropic2.png");

    // c(r) = 1/(1+exp^(0.25*r-50))
    Image pic7(pic);
    pic7 = pic7.anisotropic(3,angle);
    pic7.display_image("anisotropic c(r)=1/(1+exp^(0.25*r-50))");
    pic7.save_image("result/anisotropic3.png");
    return 0;
}
