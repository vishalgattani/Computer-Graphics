#include "load_bmp.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "SOIL/SOIL.h"
#include "Texture.h"

glm::vec3 printMat(int t,int n,glm::mat4  mat){
  glm::vec3 vals;
  //cout<<"#"<<t << ",#children " << n <<": (x,y,z) : ("<<mat[3][0]<<","<<mat[3][1]<<","<<mat[3][2]<<")"<<endl;
  //printf("\n");
  vals.x = (mat[3][0]);
  vals.y = (mat[3][1]);
  vals.z = (mat[3][2]);
  return vals;
}

glm::mat4 TRS ( vec3 ( offset ), float angle ,  float scul)
{
    glm::mat4 t = translate ( mat4(1.0), offset  );
    glm::mat4 r = rotate ( mat4(1.0), angle, vec3(0,1,0) );
    glm::mat4 s = scale (mat4(1.0), vec3(scul,scul,scul));
    return t * r * s;
}

GLuint texInit(char * texture)
{
    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glActiveTexture(GL_TEXTURE0);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
                   GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                   GL_LINEAR);

    int width, height;
    unsigned char* image = SOIL_load_image(texture, &width, &height, 0, SOIL_LOAD_RGB);
    
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
              GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image); 
    return textureID;

}

class drawObject{
public:
    static int id;
    static float rot_angle_check;
    static float disp;

    int motionID;

    float factor;
    int tagNo;
    float speed;
    GLuint vao;
    GLuint vertex_buf;
    GLuint color_buf;
    GLuint normal_buf;
    GLuint uv_buf;
    GLuint tangent_buffer;
    GLuint bitangent_buffer;
    GLuint programID;
    GLuint lightID;
    GLuint pickingProgramID,PickingMatrixID,pickingColorID;

    
    GLuint texture1;
    GLuint tex1ID;
    GLuint textureSpecular;
    GLuint texSpecularID;
    GLuint textureNormal;
    GLuint texNormalID;
    GLuint textureAmbient;
    GLuint texAmbientID;

    int lightonoff;

    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    GLuint LightID;
    GLuint ModelView3x3MatrixID ;
    GLuint ModelView3x3MatrixID2 ;
    GLuint ModelMatrixID2;
    GLuint ViewMatrixID2;

    glm::mat4 ProjectionMatrix;
    glm::mat4 ModelViewMatrix ;
    glm::mat3 ModelView3x3Matrix; 
    glm::mat4 ViewMatrix;
    glm::mat4 ModelMatrix;
    glm::mat4 Mod;
    glm::mat4 MVP;
    glm::vec3 lightPos;
    vec4 position;

    vec3 spos;

    vec3 difference_vector;

    mat4 cached_translate;
    mat4 cached_rotate;
    mat4 cached_scale;
    float scaling;
    
    mat4 translate_rotate_scale_matrix;
    mat4 worldMatrix;
    mat4 localMatrix;

    vector<drawObject*> parent;
    vector<drawObject*> children;

    vec3 object_position;

    //drawObject();
    ~drawObject();
    void fillBuffers(OBJLoader);
    void supplyShader();
    void setMatrix(mat4 trs);
    void setWorldMatrix(mat4 trs);
    void setLocalMatrix(mat4 trs);
    int getNumChildren();

    void calcMatricesFromController(Controller*,OBJLoader m,const char*,const char*);
    void calcMatrices(Controller*,OBJLoader m,const char*,const char*);
    void calcMatricesLOCALWORLD(Controller* ctrl,mat4 wrldMat,OBJLoader m,const char* vert_filename,const char* frag_filename);
    mat4 update(int motion=1, float scaling=1,float radius=5, float baseFrequency=1,vec3 diff = vec3(0,0,0));
    void releaseAttrib();
    void draw(OBJLoader);
    drawObject(int m,vec3,float angle, float scul,mat4 wrldMat,const char*,const char*,const char*,const char*,const char*,const char*);
    void appendChild(drawObject* child,drawObject* parent);
    vec3 getPosition();
    int getID();
    void setDifferenceVector(vec3 diff);
};

int drawObject::id = 0;
float drawObject::rot_angle_check=0;
float drawObject::disp=0;

int drawObject::getID(){
    return tagNo;
}
void drawObject::setDifferenceVector(vec3 diff){
    difference_vector = diff;
}

drawObject::drawObject(int m,vec3 startPos,float angle ,float scul, mat4 wrldMat,const char* texture_filename,const char* specular_file,const char* normal_file,const char* ambient_file,const char* vert_filename,const char* frag_filename){
    motionID = m;
    speed = 0.1;
    spos = startPos;
    factor = 1;
    lightonoff = 1;
    cached_translate = translate ( mat4(1.0), startPos  );
    cached_rotate = rotate ( mat4(1.0), angle, vec3(0,1,0) );
    cached_scale = scale (mat4(1.0), vec3(scul,scul,scul));
    scaling = float(scul);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //translate_rotate_scale_matrix = mat4(1.0);
    glGenBuffers(1, &vertex_buf);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
    
    glGenBuffers(1, &color_buf);
    glBindBuffer(GL_ARRAY_BUFFER, color_buf);
    
    glGenBuffers(1, &normal_buf);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buf);

    glGenBuffers(1, &uv_buf);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buf);

    glGenBuffers(1, &tangent_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, tangent_buffer);

    glGenBuffers(1, &bitangent_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, bitangent_buffer);

    //programID = LoadShadersWithG( "StandardShading.vertexshader", frag_filename, "trial_geo.shader" );
    Shader shaderid(vert_filename,frag_filename);
    MatrixID = glGetUniformLocation(shaderid.ID, "MVP");
    ViewMatrixID = glGetUniformLocation(shaderid.ID, "V");
    ModelMatrixID = glGetUniformLocation(shaderid.ID, "M");
    ViewMatrixID2 = glGetUniformLocation(shaderid.ID, "V1");
    ModelMatrixID2 = glGetUniformLocation(shaderid.ID, "M1");
    LightID = glGetUniformLocation(shaderid.ID, "LightPosition_worldspace");
    tex1ID  = glGetUniformLocation(shaderid.ID, "texture1");
    texNormalID  = glGetUniformLocation(shaderid.ID, "textureNormal");
    ModelView3x3MatrixID = glGetUniformLocation(shaderid.ID, "MV3");
    ModelView3x3MatrixID2 = glGetUniformLocation(shaderid.ID, "MV31");
    texAmbientID  = glGetUniformLocation(shaderid.ID, "textureAmbient");
    //lightpower = glGetUniformLocation(shaderid.ID, "LightPower");
    //pickingProgramID = LoadShaders( "Picking.vertexshader", "Picking.fragmentshader" );
    //PickingMatrixID = glGetUniformLocation(pickingProgramID, "MVP");
    

    //Mod = translate(startPos) ;// * scale(vec3(2,1,1));
    texture1 = loadBMP_custom(texture_filename);
    //textureSpecular = loadBMP_custom(specular_file);
    //textureNormal = loadBMP_custom(normal_file);
    //textureAmbient = loadBMP_custom(ambient_file);

    //Texture texture1;//, texture2, texture3, texture4;
    //texture1.create("./textures/worldmap.png");
    //texture1.bind(0);
    
    // load and create a texture 
    // -------------------------
    //texture1 = texInit("./textures/masterball.png");
    
    //shaderid.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    //glUniform1i(glGetUniformLocation(shaderid.ID, "texture1"), 1);

    cached_translate = translate ( mat4(1.0), startPos);
    worldMatrix = mat4(1.0);
    localMatrix = TRS(startPos,angle,scul);
    worldMatrix = wrldMat;
    id++;
    cout << "DDDEEBUGGG tag number " << id << endl;
    tagNo = id;

}

void drawObject::setMatrix(mat4 trs){
    translate_rotate_scale_matrix = trs;
}

void drawObject::setWorldMatrix(mat4 trs){
    worldMatrix = trs;
}

void drawObject::setLocalMatrix(mat4 trs){
    localMatrix = trs;
    for(int i=0;i<children.size();i++){
        children[i]->setLocalMatrix(trs);
    }
}

int drawObject::getNumChildren(){
    return children.size();
}

vec3 drawObject::getPosition(){
    return object_position;
}


void drawObject::calcMatricesFromController(Controller* ctrl, OBJLoader m,const char* vert_filename,const char* frag_filename){
    Shader shader(vert_filename, frag_filename);
    shader.use();


    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
    glBufferData(GL_ARRAY_BUFFER, m.out_vertices.size() * sizeof(glm::vec3), &m.out_vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, color_buf);
    glBufferData(GL_ARRAY_BUFFER, m.out_colors.size() * sizeof(glm::vec3), &m.out_colors[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, normal_buf);
    glBufferData(GL_ARRAY_BUFFER, m.out_normals.size() * sizeof(glm::vec3), &m.out_normals[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, uv_buf);
    //glBufferData(GL_ARRAY_BUFFER, m.planar_uvs.size() * sizeof(glm::vec2), &m.planar_uvs[0], GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, m.spherical_uvs.size() * sizeof(glm::vec2), &m.spherical_uvs[0], GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, m.cylindrical_uvs.size() * sizeof(glm::vec2), &m.cylindrical_uvs[0], GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, m.out_uv.size() * sizeof(glm::vec2), &m.out_uv[0], GL_STATIC_DRAW);
        

    glBindBuffer(GL_ARRAY_BUFFER, tangent_buffer);
    glBufferData(GL_ARRAY_BUFFER, m.tangents.size() * sizeof(glm::vec3), &m.tangents[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, bitangent_buffer);
    glBufferData(GL_ARRAY_BUFFER, m.bitangents.size() * sizeof(glm::vec3), &m.bitangents[0], GL_STATIC_DRAW);

    ProjectionMatrix = ctrl->getProjectionMatrix();
    ViewMatrix = ctrl->getViewMatrix();

    float rotAngle = (rand() % 360);

    
     
    if (glfwGetKey( ctrl->window, GLFW_KEY_L ) == GLFW_PRESS){
        //cout << rot_angle_check << ","<< speed<< "," <<tagNo << endl << endl;
            rot_angle_check = rot_angle_check;
    }
    else{
        rot_angle_check = rot_angle_check+0.001f;
        if(rot_angle_check>360) rot_angle_check = 0;
    }
    

    
    // Convert "i", the integer mesh ID, into an RGB color
                int r = (tagNo & 0x000000FF) >>  0;
                int g = (tagNo & 0x0000FF00) >>  8;
                int b = (tagNo & 0x00FF0000) >> 16;
                glUseProgram(pickingProgramID);
    
        
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    //modelMatrix = glm::translate(modelMatrix, -spos);
    //modelMatrix = cached_translate*cached_rotate*cached_scale;
    //translate_rotate_scale_matrix = modelMatrix;
    modelMatrix = translate_rotate_scale_matrix;
    modelMatrix = localMatrix;
    glm::vec3 check = printMat(tagNo,children.size(),modelMatrix);
    object_position = check;
    //cout << "second  ";


    //glm::mat4 modelMatrix = TRS(-vec3(4,4,4),0,1);
    //modelMatrix = glm::rotate(modelMatrix, rot_angle_check, glm::vec3(0.0f, 1.0f, 0.0f));
    

    /*if(tagNo==4) {
            modelMatrix = glm::rotate(modelMatrix, rot_angle_check, glm::vec3(0.0f, 1.0f, 0.0f));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0,disp, 0));
            
            disp = disp+(factor*0.005f);
            if(disp>5) factor=-1;
            if(disp<-5) factor=1;

            

    }
    if(tagNo==2) {
            //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
            modelMatrix = glm::translate(modelMatrix, spos);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(3,0,-3));
            //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(3*cos(rot_angle_check),disp, 3*sin(rot_angle_check)));
            modelMatrix = glm::rotate(modelMatrix, rot_angle_check, glm::vec3(0.0f, 1.0f, 0.0f));
            //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
    }*/
    
        
    
    //modelMatrix = glm::translate(modelMatrix, spos);
    ModelMatrix = ctrl->getRotationMatrix()*modelMatrix;
    position = ModelMatrix * vec4(0,0,0,1);
    //cout << tagNo << ": " << position.x << "," << position.y << "," << position.z << endl;
    MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
    glUniformMatrix4fv(PickingMatrixID, 1, GL_FALSE, &MVP[0][0]);
    // OpenGL expects colors to be in [0,1], so divide by 255.
    glUniform4f(pickingColorID, r/255.0f, g/255.0f, b/255.0f, 1.0f);
    ModelViewMatrix = ViewMatrix * ModelMatrix;
    ModelView3x3Matrix = mat3(ModelViewMatrix);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
    glVertexAttribPointer(
        0,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

     // 4th attribute buffer : uvs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buf);
    glVertexAttribPointer(
        1,                                // attribute
        2,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buf);
    glVertexAttribPointer(
        2,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

   

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, tangent_buffer);
    glVertexAttribPointer(
        4,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    glEnableVertexAttribArray(5);
    glBindBuffer(GL_ARRAY_BUFFER, bitangent_buffer);
    glVertexAttribPointer(
        5,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );
    shader.use();
    shader.setMat4("model", ModelMatrix);
    shader.setMat4("view", ViewMatrix);
    shader.setMat4("projection", ProjectionMatrix);
    shader.setMat4("MVP", MVP);
    shader.setMat3("MV3",ModelView3x3Matrix);
    glm::vec3 lightPos(8,5,8);
    //glm::vec3 lightPos(object_position.x+2,object_position.y+5,object_position.z+3);
    shader.setVec3("LightPosition_worldspace",lightPos);

    shader.setInt("LightOnOff",lightonoff);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    //glUniform1i(tex1ID, 0);
    /*glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureSpecular);
    glUniform1i(texSpecularID, 1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textureNormal);
    glUniform1i(texNormalID, 2);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, textureAmbient);
    glUniform1i(texAmbientID, 3);
    */
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Read the pixel at the center of the screen.
    // You can also use glfwGetMousePos().
    // Ultra-mega-over slow too, even for 1 pixel, 
    // because the framebuffer is on the GPU.
    unsigned char data[4];
    glReadPixels(1024/2, 768/2,1,1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    int pickedID =     data[0] + 
                data[1] * 256 +
                data[2] * 256*256;
}


void drawObject::calcMatrices(Controller* ctrl,OBJLoader m,const char* vert_filename,const char* frag_filename){
    Shader shader(vert_filename, frag_filename);
    shader.use();
    float previous = glfwGetTime();
    //localMatrix = TRS(vec3(0,0,0),(previous),1.0f);
    localMatrix = cached_translate*localMatrix;
    worldMatrix = localMatrix;
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
    glBufferData(GL_ARRAY_BUFFER, m.out_vertices.size() * sizeof(glm::vec3), &m.out_vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, color_buf);
    glBufferData(GL_ARRAY_BUFFER, m.out_colors.size() * sizeof(glm::vec3), &m.out_colors[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, normal_buf);
    glBufferData(GL_ARRAY_BUFFER, m.out_normals.size() * sizeof(glm::vec3), &m.out_normals[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, uv_buf);
    //glBufferData(GL_ARRAY_BUFFER, m.planar_uvs.size() * sizeof(glm::vec2), &m.planar_uvs[0], GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, m.spherical_uvs.size() * sizeof(glm::vec2), &m.spherical_uvs[0], GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, m.cylindrical_uvs.size() * sizeof(glm::vec2), &m.cylindrical_uvs[0], GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, m.out_uv.size() * sizeof(glm::vec2), &m.out_uv[0], GL_STATIC_DRAW);
        

    glBindBuffer(GL_ARRAY_BUFFER, tangent_buffer);
    glBufferData(GL_ARRAY_BUFFER, m.tangents.size() * sizeof(glm::vec3), &m.tangents[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, bitangent_buffer);
    glBufferData(GL_ARRAY_BUFFER, m.bitangents.size() * sizeof(glm::vec3), &m.bitangents[0], GL_STATIC_DRAW);

    ProjectionMatrix = ctrl->getProjectionMatrix();
    ViewMatrix = ctrl->getViewMatrix();

    float rotAngle = (rand() % 360);

    
     
    if (glfwGetKey( ctrl->window, GLFW_KEY_L ) == GLFW_PRESS){
        //cout << rot_angle_check << ","<< speed<< "," <<tagNo << endl << endl;
            rot_angle_check = rot_angle_check;
    }
    else{
        rot_angle_check = rot_angle_check+0.001f;
        if(rot_angle_check>360) rot_angle_check = 0;
    }
    

    
    // Convert "i", the integer mesh ID, into an RGB color
                int r = (tagNo & 0x000000FF) >>  0;
                int g = (tagNo & 0x0000FF00) >>  8;
                int b = (tagNo & 0x00FF0000) >> 16;
                glUseProgram(pickingProgramID);
    
        
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    //modelMatrix = glm::translate(modelMatrix, -spos);
    //modelMatrix = cached_translate*cached_rotate*cached_scale;
    //translate_rotate_scale_matrix = modelMatrix;
    modelMatrix = translate_rotate_scale_matrix;
    modelMatrix = localMatrix;
    glm::vec3 check = printMat(tagNo,children.size(),modelMatrix);
    object_position = check;
    //cout << "second  ";


    //glm::mat4 modelMatrix = TRS(-vec3(4,4,4),0,1);
    //modelMatrix = glm::rotate(modelMatrix, rot_angle_check, glm::vec3(0.0f, 1.0f, 0.0f));
    

    /*if(tagNo==4) {
            modelMatrix = glm::rotate(modelMatrix, rot_angle_check, glm::vec3(0.0f, 1.0f, 0.0f));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0,disp, 0));
            
            disp = disp+(factor*0.005f);
            if(disp>5) factor=-1;
            if(disp<-5) factor=1;

            

    }
    if(tagNo==2) {
            //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
            modelMatrix = glm::translate(modelMatrix, spos);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(3,0,-3));
            //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(3*cos(rot_angle_check),disp, 3*sin(rot_angle_check)));
            modelMatrix = glm::rotate(modelMatrix, rot_angle_check, glm::vec3(0.0f, 1.0f, 0.0f));
            //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
    }*/
    
        
    
    //modelMatrix = glm::translate(modelMatrix, spos);
    ModelMatrix = ctrl->getRotationMatrix()*modelMatrix;
    position = ModelMatrix * vec4(0,0,0,1);
    //cout << tagNo << ": " << position.x << "," << position.y << "," << position.z << endl;
    MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
    glUniformMatrix4fv(PickingMatrixID, 1, GL_FALSE, &MVP[0][0]);
    // OpenGL expects colors to be in [0,1], so divide by 255.
    glUniform4f(pickingColorID, r/255.0f, g/255.0f, b/255.0f, 1.0f);
    ModelViewMatrix = ViewMatrix * ModelMatrix;
    ModelView3x3Matrix = mat3(ModelViewMatrix);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
    glVertexAttribPointer(
        0,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

     // 4th attribute buffer : uvs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buf);
    glVertexAttribPointer(
        1,                                // attribute
        2,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buf);
    glVertexAttribPointer(
        2,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

   

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, tangent_buffer);
    glVertexAttribPointer(
        4,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    glEnableVertexAttribArray(5);
    glBindBuffer(GL_ARRAY_BUFFER, bitangent_buffer);
    glVertexAttribPointer(
        5,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );
    shader.use();
    shader.setMat4("model", ModelMatrix);
    shader.setMat4("view", ViewMatrix);
    shader.setMat4("projection", ProjectionMatrix);
    shader.setMat4("MVP", MVP);
    shader.setMat3("MV3",ModelView3x3Matrix);
    glm::vec3 lightPos(8,5,8);
    //glm::vec3 lightPos(object_position.x+2,object_position.y+5,object_position.z+3);
    shader.setVec3("LightPosition_worldspace",lightPos);

    shader.setInt("LightOnOff",lightonoff);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    //glUniform1i(tex1ID, 0);
    /*glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureSpecular);
    glUniform1i(texSpecularID, 1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textureNormal);
    glUniform1i(texNormalID, 2);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, textureAmbient);
    glUniform1i(texAmbientID, 3);
    */
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Read the pixel at the center of the screen.
    // You can also use glfwGetMousePos().
    // Ultra-mega-over slow too, even for 1 pixel, 
    // because the framebuffer is on the GPU.
    unsigned char data[4];
    glReadPixels(1024/2, 768/2,1,1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    int pickedID =     data[0] + 
                data[1] * 256 +
                data[2] * 256*256;
}

mat4 drawObject::update(int motion, float scaling,float radius, float baseFrequency,vec3 diff){
    float previous = glfwGetTime();
    mat4 thisMatrix = cached_translate;
    if(motionID==1){
        mat4 c_translate = translate ( cached_translate, vec3(4*radius*cos(baseFrequency*previous),0,2*radius*sin(baseFrequency*previous)));
        mat4 c_rotate = rotate ( mat4(1.0), previous, vec3(0.5,1,0) );
        mat4 c_scale = scale (mat4(1.0), vec3(scaling,scaling,scaling));
        thisMatrix = c_translate*c_rotate*c_scale;
    }
    else if(motionID==2){
        mat4 c_translate = translate ( cached_translate, vec3(0,0,0));
        mat4 c_rotate = rotate ( mat4(1.0), previous, vec3(0,1,0) );
        mat4 c_scale = scale (mat4(1.0), vec3(1,1,1));
        thisMatrix = c_translate*c_rotate*c_scale;
    }
    else if(motionID==3){
        

        mat4 c_translate = translate ( cached_translate,-diff*2.0f);
        float wobble = 0.02f;
        c_translate = translate (c_translate,vec3(wobble*cos(rand()%360),0,wobble*cos(rand()%360)));  
        mat4 c_rotate = rotate ( mat4(1.0), previous, vec3(0,1,0) );
        mat4 c_scale = scale (mat4(1.0), vec3(1,1,1));
        thisMatrix = c_translate*c_rotate*c_scale;
    }
    else if(motionID==4){
        mat4 c_translate = translate ( cached_translate,vec3(0,abs(1*cos(3*baseFrequency*previous)),0));
        mat4 c_rotate = rotate ( mat4(1.0), previous, vec3(0,1,0) );
        mat4 c_scale = scale (mat4(1.0), vec3(0.5,0.5,0.5));
        thisMatrix = c_translate*c_rotate*c_scale;
    }

    else if(motionID==0){
        mat4 c_translate = translate ( cached_translate, vec3(0,0,0));
        mat4 c_rotate = rotate ( mat4(1.0), previous, vec3(0,1,0) );
        mat4 c_scale = scale (mat4(1.0), vec3(5,5,5));
        thisMatrix = c_scale;
    }
    else thisMatrix = mat4(1.0);
    return thisMatrix;
}




void drawObject::calcMatricesLOCALWORLD(Controller* ctrl,mat4 wrldMat,OBJLoader m,const char* vert_filename,const char* frag_filename){
    Shader shader(vert_filename, frag_filename);
    shader.use();
    float previous = glfwGetTime();
    worldMatrix = wrldMat;
    mat4 temp = update(motionID,scaling,5,1,difference_vector);
    //localMatrix = worldMatrix*temp;
    localMatrix = localMatrix;
    localMatrix *= worldMatrix*temp;
    
    //localMatrix = worldMatrix*TRS(vec3(0,0,0),(previous),1.0f);
    

    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
    glBufferData(GL_ARRAY_BUFFER, m.out_vertices.size() * sizeof(glm::vec3), &m.out_vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, color_buf);
    glBufferData(GL_ARRAY_BUFFER, m.out_colors.size() * sizeof(glm::vec3), &m.out_colors[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, normal_buf);
    glBufferData(GL_ARRAY_BUFFER, m.out_normals.size() * sizeof(glm::vec3), &m.out_normals[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, uv_buf);
    //glBufferData(GL_ARRAY_BUFFER, m.planar_uvs.size() * sizeof(glm::vec2), &m.planar_uvs[0], GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, m.spherical_uvs.size() * sizeof(glm::vec2), &m.spherical_uvs[0], GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, m.cylindrical_uvs.size() * sizeof(glm::vec2), &m.cylindrical_uvs[0], GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, m.out_uv.size() * sizeof(glm::vec2), &m.out_uv[0], GL_STATIC_DRAW);
    if(motionID==3) setWorldMatrix(localMatrix);    

    glBindBuffer(GL_ARRAY_BUFFER, tangent_buffer);
    glBufferData(GL_ARRAY_BUFFER, m.tangents.size() * sizeof(glm::vec3), &m.tangents[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, bitangent_buffer);
    glBufferData(GL_ARRAY_BUFFER, m.bitangents.size() * sizeof(glm::vec3), &m.bitangents[0], GL_STATIC_DRAW);

    ProjectionMatrix = ctrl->getProjectionMatrix();
    ViewMatrix = ctrl->getViewMatrix();

    float rotAngle = (rand() % 360);

    
     
    if (glfwGetKey( ctrl->window, GLFW_KEY_L ) == GLFW_PRESS){
        //cout << rot_angle_check << ","<< speed<< "," <<tagNo << endl << endl;
            rot_angle_check = rot_angle_check;
    }
    else{
        rot_angle_check = rot_angle_check+0.001f;
        if(rot_angle_check>360) rot_angle_check = 0;
    }
    

    
    // Convert "i", the integer mesh ID, into an RGB color
                int r = (tagNo & 0x000000FF) >>  0;
                int g = (tagNo & 0x0000FF00) >>  8;
                int b = (tagNo & 0x00FF0000) >> 16;
                glUseProgram(pickingProgramID);
    
        
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    //modelMatrix = glm::translate(modelMatrix, -spos);
    //modelMatrix = cached_translate*cached_rotate*cached_scale;
    //translate_rotate_scale_matrix = modelMatrix;
    modelMatrix = translate_rotate_scale_matrix;
    modelMatrix = localMatrix;
    glm::vec3 check = printMat(tagNo,children.size(),modelMatrix);
    object_position = check;
    //cout << "second  ";


    //glm::mat4 modelMatrix = TRS(-vec3(4,4,4),0,1);
    //modelMatrix = glm::rotate(modelMatrix, rot_angle_check, glm::vec3(0.0f, 1.0f, 0.0f));
    

    /*if(tagNo==4) {
            modelMatrix = glm::rotate(modelMatrix, rot_angle_check, glm::vec3(0.0f, 1.0f, 0.0f));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0,disp, 0));
            
            disp = disp+(factor*0.005f);
            if(disp>5) factor=-1;
            if(disp<-5) factor=1;

            

    }
    if(tagNo==2) {
            //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
            modelMatrix = glm::translate(modelMatrix, spos);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(3,0,-3));
            //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(3*cos(rot_angle_check),disp, 3*sin(rot_angle_check)));
            modelMatrix = glm::rotate(modelMatrix, rot_angle_check, glm::vec3(0.0f, 1.0f, 0.0f));
            //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
    }*/
    
        
    
    //modelMatrix = glm::translate(modelMatrix, spos);
    ModelMatrix = ctrl->getRotationMatrix()*modelMatrix;
    position = ModelMatrix * vec4(0,0,0,1);
    //cout << tagNo << ": " << position.x << "," << position.y << "," << position.z << endl;
    MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
    glUniformMatrix4fv(PickingMatrixID, 1, GL_FALSE, &MVP[0][0]);
    // OpenGL expects colors to be in [0,1], so divide by 255.
    glUniform4f(pickingColorID, r/255.0f, g/255.0f, b/255.0f, 1.0f);
    ModelViewMatrix = ViewMatrix * ModelMatrix;
    ModelView3x3Matrix = mat3(ModelViewMatrix);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
    glVertexAttribPointer(
        0,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

     // 4th attribute buffer : uvs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buf);
    glVertexAttribPointer(
        1,                                // attribute
        2,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buf);
    glVertexAttribPointer(
        2,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

   

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, tangent_buffer);
    glVertexAttribPointer(
        4,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    glEnableVertexAttribArray(5);
    glBindBuffer(GL_ARRAY_BUFFER, bitangent_buffer);
    glVertexAttribPointer(
        5,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );
    shader.use();
    shader.setMat4("model", ModelMatrix);
    shader.setMat4("view", ViewMatrix);
    shader.setMat4("projection", ProjectionMatrix);
    shader.setMat4("MVP", MVP);
    shader.setMat3("MV3",ModelView3x3Matrix);
    glm::vec3 lightPos(8,5,8);
    //glm::vec3 lightPos(object_position.x+2,object_position.y+5,object_position.z+3);
    shader.setVec3("LightPosition_worldspace",lightPos);
    shader.setInt("LightOnOff",lightonoff);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    //glUniform1i(tex1ID, 0);
    /*glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureSpecular);
    glUniform1i(texSpecularID, 1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textureNormal);
    glUniform1i(texNormalID, 2);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, textureAmbient);
    glUniform1i(texAmbientID, 3);
    */
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Read the pixel at the center of the screen.
    // You can also use glfwGetMousePos().
    // Ultra-mega-over slow too, even for 1 pixel, 
    // because the framebuffer is on the GPU.
    unsigned char data[4];
    glReadPixels(1024/2, 768/2,1,1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    int pickedID =     data[0] + 
                data[1] * 256 +
                data[2] * 256*256;
}



void drawObject::draw(OBJLoader m){
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, m.out_vertices.size());
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
    glDisableVertexAttribArray(5);
}

void drawObject::releaseAttrib(){
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
    glDisableVertexAttribArray(5);
}

void drawObject::appendChild(drawObject* child,drawObject* parent){
    child->setWorldMatrix(parent->worldMatrix);
    children.push_back(child);
}

