library('RNiftyReg')

regImgToTemplate <- function(imgFilename, templateFilename){
  sourceImage <- readNifti(imgFilename)
  templateImage <- readNifti(templateFilename)
  
  transform <- niftyreg(sourceImage,targetImage)
}

testImgReg <- function(imgFilename){
  sourceImage <- readNifti(imgFilename)

  # Set dummy transformations and print to screen
  transV <- (runif(3) - 0.5) * 20 # Translations up to 10
  scaleV <- sqrt(sqrt(runif(3) + 0.5)) # Tends to the [0.8, 1.2] range
  rotV   <- runif(3) - 0.5        # -0.5 to + 0.5 radians (about 30 degrees max)
  
  cat("Translation vector: ", transV, "\n")
  cat("Rotation angles: ", rotV, "\n")
  
  # Build the affine transform and apply it to the image
  afT = buildAffine(translation=transV, scales=c(1,1,1),angles=rotV, 
                    source=sourceImage)
  targetImage <- applyTransform(afT,sourceImage)
  
  transform <- niftyreg(sourceImage,targetImage)
  
  print(decomposeAffine(forward(transform)))
  print(decomposeAffine(reverse(transform)))
  
}