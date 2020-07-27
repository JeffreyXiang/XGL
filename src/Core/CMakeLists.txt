Xi_getTargetNameRel(GLAD_NAME libraries/GLAD)
Xi_getTargetNameRel(STB_IMAGE_NAME libraries/stb_image)
Xi_addTarget(MODE STATIC LIBS glfw3dll ${GLAD_NAME} ${STB_IMAGE_NAME})