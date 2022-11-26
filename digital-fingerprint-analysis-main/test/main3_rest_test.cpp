#include "../include/part3.hpp"

int main(){
//****************************************************************
    //This part is to build matrix K and test on the clean pic, then see if the matrix K is correct or not

    //build K matrix
    MatrixXf K = MatrixXf::Zero(3600,3136);
    for (int i=0;i<56;i++){
        for (int j=0;j<56;j++){
            K((i+0)*60+j+4,i*56+j) = 1/7.;
            K((i+1)*60+j+3,i*56+j) = 1/7.;
            K((i+2)*60+j+2,i*56+j) = 1/7.;
            K((i+3)*60+j+1,i*56+j) = 1/7.;
            K((i+3)*60+j+2,i*56+j) = 1/7.;
            K((i+4)*60+j+0,i*56+j) = 1/7.;
            K((i+4)*60+j+2,i*56+j) = 1/7.;
        }
    }

    //original clean pic
    //Mat_<float>F = imread("../picture/clean_blur_ex.png", 0);
    Mat F = imread("picture/clean_blur_ex.png", IMREAD_GRAYSCALE);
    F.convertTo(F,CV_32FC1,1/255.0);
    //imshow("original", F);

    //make mat of original pic to one column
    F = F.reshape(1,1).t();

    cout<<"F rows: "<<F.rows<<endl;
    cout<<"F cols: "<<F.cols<<endl;


    //transfer mat F to matrix f so that we can use SVD decomposition built in eigen lib
    Matrix<float,Dynamic, Dynamic> f;
    cv2eigen(F,f);

    //K transpose and save matrix in itself
    K.transposeInPlace();
    cout<<"K rows:"<<K.rows()<<endl;

    //matrix K times matrix f
    MatrixXf tran = K*f;
    cout<<"tran rows:"<<tran.rows()<<endl;
    cout<<"tran cols:"<<tran.cols()<<endl;

    //transfer matrix of the result back to mat so that we can show the pic of our result
    Mat fb;
    eigen2cv(tran,fb);

    //one column mat to the mat form of pic
    fb = fb.reshape(1,56);

    //show the result
    imshow("corrupted", fb);
    imwrite("result/corrupted.png", fb*255);
    waitKey();


    //****************************************************************
    //This part is to use different methods to get clean image based on the K we built above.

    Image corrupted("picture/corrupted_blur_ex.png");
    corrupted.int_to_float();


    // SVD linear solver
    Mat svd = linearsolver(corrupted,K, LinearSolver_SVD);
    Image SVD(svd);
    SVD.display_image("SVD");
    SVD.save_image("result/SVD.png");

    // QR linear solver
    Mat qr = linearsolver(corrupted,K, LinearSolver_QR);
    Image QR(qr);
    QR.display_image("QR");
    QR.save_image("result/QR.png");

    Mat Fb = imread("picture/corrupted_blur_ex.png", IMREAD_GRAYSCALE);
    Fb.convertTo(F,CV_32FC1,1/255.0);
    Fb = Fb.reshape(1,1).t();

    cout<<"Fb rows: "<<Fb.rows<<endl;
    cout<<"Fb cols: "<<Fb.cols<<endl;

    Matrix<float,Dynamic, Dynamic> FB;

    cv2eigen(Fb,FB);

    //build 3600*1 matirx of all zeros
    MatrixXf zero_add = MatrixXf::Zero(3600,1);
    MatrixXf left;
    left.resize(FB.rows()+3600,1);
    cout<<"left rows: "<<left.rows()<<endl;
    //put matrix of corrupted pic and zero matrix together
    left<<FB,
            zero_add;

    //define lamda
    float lamda = 15;
    //lamda times identify matrix with size 3600*3600
    MatrixXf iden_add = lamda*MatrixXf::Identity(3600,3600);
    MatrixXf right;
    right.resize(K.rows()+3600,K.cols()+3600);
    cout<<"right rows: "<<right.rows()<<endl;
    //put K matrix and matrix of lamda times identity together
    right<<K,
            iden_add;

    //************ SVD ***************
    Matrix<float,Dynamic, Dynamic> result = right.bdcSvd(ComputeThinU | ComputeThinV).solve(left);

    //get the top 3600 rows of the result
    Matrix<float,Dynamic, Dynamic> Result = result.topRows(3600);

    //transfer matirx of result to mat so that we can show the pic of result
    Mat res;
    eigen2cv(Result,res);

    //back to the form of pic mat
    res = res.reshape(1,60);

    imshow("well posed result", res);
    waitKey();
    res.convertTo(res,CV_8UC1,255);
    imwrite("result/lamda15.png",res);


    return 0;
}

