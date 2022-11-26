import cv2
import numpy as np
from matplotlib import pyplot as plt

  
img = cv2.imread('../picture/clean_finger.png')
rows, cols, ch = img.shape

#  Selecting the points that will create the affine matrix 

sourcePoints = np.float32([[20, 50],[200, 50], [50, 200]])
targetPoints = np.float32([[10,100],[200,50],[100,250]])

# The affine matrix needed for the image warp
M = cv2.getAffineTransform(sourcePoints,targetPoints)

######Image warp######

# different interpolation can be done by changing the flags cv2.INTER
# for linear interpolation : cv2.INTER_LINEAR
# for bicubic interpolation : cv2.INTER_CUBIC
# for the nearest neighbor interpolation : cv2.INTER_NEAREST

img_warp = cv2.warpAffine(img, M, (cols, rows),cv2.INTER_CUBIC, borderMode=cv2.BORDER_CONSTANT,borderValue=(255, 255, 255))

plt.imshow(img_warp),plt.title('Image warp')
plt.savefig("../result/Imagewarp.png")
plt.close()
### Perspective Image Warp ####

## choosing the points for the perspective affine matrix 
sourcePoints_perspective = np.float32([[50,65],[170,52],[30,260],[250,250]])
targetPoints_perspective = np.float32([[0,0],[110,0],[0,210],[210,210]])

# The matrix that performs the persepctive affine transformation
M_prespective = cv2.getPerspectiveTransform(sourcePoints_perspective,targetPoints_perspective)

## The perspective Transformation
img_warp_prespective = cv2.warpPerspective(img_warp,M_prespective,(cols,rows), cv2.INTER_CUBIC, borderMode=cv2.BORDER_CONSTANT,borderValue=(255, 255, 255))

plt.imshow(img_warp_prespective),plt.title('normal warp + perspective warp')
plt.savefig("../result/Imagewarp_Perspective.png")
plt.close()

## TRANSLATION
h,w = img_warp_prespective.shape[:2]
M_Translation =np.float32([[1,0,60],[0,1,60]])
img_translated=cv2.warpAffine(img_warp_prespective,M_Translation,(w,h),cv2.INTER_CUBIC, borderMode=cv2.BORDER_CONSTANT,borderValue=(255, 255, 255))
plt.figure()
plt.imshow(img_translated),plt.title("Translation of the Warped Image")
plt.savefig("../result/translated.png")
plt.close()

## ROTATION

## finding the center of the image and rotate by a certain degree around the center 

image_center = tuple(np.array(img_translated.shape[1::-1]) / 2)
rot_mat = cv2.getRotationMatrix2D(image_center, 15, 1.0)
img_rotated = cv2.warpAffine(img_translated, rot_mat, img_translated.shape[1::-1], cv2.INTER_CUBIC,borderMode=cv2.BORDER_CONSTANT,borderValue=(255, 255, 255))

plt.figure()
plt.imshow(img_rotated),plt.title("Image Rotated by 15 degrees")
plt.savefig("../result/rotated.png")
plt.close()

## Final Translation to match the image frame
M_translation2 =np.float32([[1,0,0],[0,1,30]])
result=cv2.warpAffine(img_rotated,M_translation2,(w,h),cv2.INTER_CUBIC,borderMode=cv2.BORDER_CONSTANT,borderValue=(255, 255, 255))
plt.figure()
plt.imshow(result),plt.title("Result")
plt.savefig("../result/result.png")
plt.close()

### Downsampling the image to see the effect of different interpolation

scale_percent = 35
width_small = int(result.shape[1] * scale_percent / 100)
height_small = int(result.shape[0] * scale_percent / 100)
dim = (width_small, height_small)
img_small = cv2.resize(result,dim)

# upsampling back to original size
img_nn  = cv2.resize(img_small, (result.shape[1],result.shape[0]) , interpolation = cv2.INTER_NEAREST)
img_bil = cv2.resize(img_small, (result.shape[1],result.shape[0]), interpolation = cv2.INTER_LINEAR)
img_bic = cv2.resize(img_small, (result.shape[1],result.shape[0]), interpolation = cv2.INTER_CUBIC)


plt.figure(figsize=(14,10))
plt.subplot(2,2,1)
plt.imshow(img_small)
plt.title('Downsampled')
plt.subplot(2,2,2)
plt.imshow(img_nn)
plt.title('Nearest Neighbor')
plt.subplot(2,2,3)
plt.imshow(img_bil)
plt.title('Bilinear')
plt.subplot(2,2,4)
plt.imshow(img_bic)
plt.title('Bicubic')
plt.savefig("../result/interpolations.png")
plt.close()