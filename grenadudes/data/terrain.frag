// fragment shader for rendering terrain

uniform sampler2D txtSolid;   // solid terrain image texture
uniform sampler2D txtKill;    // kill terrain image texture
uniform sampler2D txtWeak;    // weak terrain image texture
uniform sampler2D txtData;    // terrain data texture
uniform float sizeX;          // size of rendering space
uniform float sizeY;          // ^
uniform float screenWidth;    // width of the screen in pixels
uniform float screenHeight;   // ^
uniform float cameraX;
uniform float cameraY;

void main() {
    // calculate texture lookup coordinates
    vec4 dataCoord;
    dataCoord.x = gl_FragCoord.x + cameraX;
    dataCoord.y = gl_FragCoord.y - cameraY;
    vec4 dataTexCoord;
    dataTexCoord.x = dataCoord.x / sizeX;
    dataTexCoord.y = (screenHeight - dataCoord.y) / sizeY; // y needs to be flipped for some reason
    
    
    
    if (dataCoord.x >= cameraX && screenHeight - dataCoord.y >= cameraY && dataCoord.x < cameraX + screenWidth && screenHeight - dataCoord.y < cameraY + screenHeight) {
        // lookup terrain type in map file
        vec4 dataPixel = texture2D(txtData, dataTexCoord.xy);
        dataPixel.xyza *= 255;
        vec4 pixel = vec4(0.0, 0.0, 0.0, 1.0);
        
        // sample from correct terrain texture 
        if (dataPixel.r == 255 && dataPixel.g == 255 && dataPixel.b == 255) {
            pixel = texture2D(txtSolid, gl_TexCoord[0].xy);
            gl_FragColor = gl_Color * pixel;
        }
        else if (dataPixel.r == 0 && dataPixel.g == 128 && dataPixel.b == 128) {
            pixel = texture2D(txtKill, gl_TexCoord[0].xy);
            gl_FragColor = gl_Color * pixel;
            gl_FragColor.a = 0.5;
        }
        else if (dataPixel.r == 127 && dataPixel.g == 127 && dataPixel.b == 127) {
            pixel = texture2D(txtWeak, gl_TexCoord[0].xy);
            gl_FragColor = gl_Color * pixel;
        } else
            gl_FragColor.a = 0;
    }
}