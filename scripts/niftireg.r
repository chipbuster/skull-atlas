library('RNiftyReg')

regImgToTemplate <- function(imgFilename, templateFilename){
  sourceImage <- readNifti(imgFilename)
  templateImage <- readNifti(templateFilename)
  
  transform <- niftyreg(sourceImage,targetImage)
}

testImgReg <- function(imgFilename){
  sourceImage <- readNifti(imgFilename)

  # Set dummy transformations and print to screen
  transV <- (runif(3) - 0.5) * 100 # Translations up to 50
  scaleV <- 1 +                     
  rotV   <- runif(3) - 0.5        # -0.5 to + 0.5 radians (about 30 degrees max)
  
  cat("Translation vector: ", transV, "\n")
  cat("Scale vector: ", scaleV, "\n")
  cat("Rotation angles: ", rotV, "\n")
  
  # Build the affine transform and apply it to the image
  afT = buildAffine(translation=transV, scales=scaleV,angles=rotV, 
                    source=sourceImage)
  targetImage <- applyTransform(afT,sourceImage)
  
  transform <- niftyreg(sourceImage,targetImage)
  
  decomposed = decomposeAffine(forward(transform))
  
  deltaTrans = transV - decomposed$translation
  deltaScale = scaleV - decomposed$scales
  deltaAngle = rotV - decomposed$angles
  
  return(list(deltaTrans,deltaScale,deltaAngle))
}