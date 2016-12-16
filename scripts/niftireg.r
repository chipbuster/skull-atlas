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
  scaleV <- c(1,1,1)    # NO SCALING!
  rotV   <- runif(3) - 0.5        # -0.5 to + 0.5 radians (about 30 degrees max)
  
#  cat("Translation vector: ", transV, "\n")
#  cat("Scale vector: ", scaleV, "\n")
#  cat("Rotation angles: ", rotV, "\n")
  
  # Build the affine transform and apply it to the image
  afT = buildAffine(translation=transV, scales=scaleV,angles=rotV, 
                    source=sourceImage)
  targetImage <- applyTransform(afT,sourceImage)
  
  transform <- niftyreg(sourceImage,targetImage)
  
  decomposed = decomposeAffine(forward(transform))
  
  deltaTrans = transV - decomposed$translation
  deltaScale = scaleV - decomposed$scales
  deltaAngle = rotV - decomposed$angles
 
  results = data.frame()
  results['Translation','OrigX'] = transV[1]
  results['Translation','OrigY'] = transV[2]
  results['Translation','OrigZ'] = transV[3]
  results['Translation','CalcX'] = decomposed$translation[1]
  results['Translation','CalcY'] = decomposed$translation[2]
  results['Translation','CalcZ'] = decomposed$translation[3]
  results['Translation','deltaX'] = deltaTrans[1]
  results['Translation','deltaY'] = deltaTrans[2]
  results['Translation','deltaZ'] = deltaTrans[3]

  results['Scaling','OrigX'] = scaleV[1]
  results['Scaling','OrigY'] = scaleV[2]
  results['Scaling','OrigZ'] = scaleV[3]
  results['Scaling','CalcX'] = decomposed$scales[1]
  results['Scaling','CalcY'] = decomposed$scales[2]
  results['Scaling','CalcZ'] = decomposed$scales[3]
  results['Scaling','deltaX'] = deltaScale[1]
  results['Scaling','deltaY'] = deltaScale[2]
  results['Scaling','deltaZ'] = deltaScale[3]

  results['Rotation','OrigX'] = rotV[1]
  results['Rotation','OrigY'] = rotV[2]
  results['Rotation','OrigZ'] = rotV[3]
  results['Rotation','CalcX'] = decomposed$angles[1]
  results['Rotation','CalcY'] = decomposed$angles[2]
  results['Rotation','CalcZ'] = decomposed$angles[3]
  results['Rotation','deltaX'] = deltaAngle[1]
  results['Rotation','deltaY'] = deltaAngle[2]
  results['Rotation','deltaZ'] = deltaAngle[3]

  print(results)

  return(results)
}
