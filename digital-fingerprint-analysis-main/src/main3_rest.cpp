#include "../include/part3.hpp"

Mat linearsolver(const Image &img,MatrixXf& K, LinearSolverType type ){
    Mat cor = img.get_matrice();

    cor = cor.reshape(1,1).t();


    Matrix<float,Dynamic, Dynamic> Cor;

    cv2eigen(cor,Cor);

    Matrix<float,Dynamic, Dynamic> result;

    switch (type) {
        case LinearSolver_SVD:
            result = K.bdcSvd(ComputeThinU | ComputeThinV).solve(Cor);
            break;
        case LinearSolver_QR:
            result = K.colPivHouseholderQr().solve(Cor);
            break;

        default:
            result = K.bdcSvd(ComputeThinU | ComputeThinV).solve(Cor);
            break;
    }


    Mat res;
    eigen2cv(result,res);

    res = res.reshape(1,60);

    return res;
}

