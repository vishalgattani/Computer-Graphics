#define NONE 0
#define CYLINDER 1
#define SPHERE 2
#define CUBE 3

#include "../include/shader.hpp"

void printVec3(const char* ve,vec3 v){
  cout<< ve << " : (x,y,z) : ("<<v.x<<","<<v.y<<","<<v.z<<")"<<endl;
    
}

void printMatrixOnly(glm::mat4  mat){
  glm::vec3 vals;
  cout<<": (x,y,z) : ("<<mat[3][0]<<","<<mat[3][1]<<","<<mat[3][2]<<")"<<endl;
  //printf("\n");
  vals.x = (mat[3][0]);
  vals.y = (mat[3][1]);
  vals.z = (mat[3][2]);
  //return vals;
}


float baseFrequency = 0.75; //Hz

drawObject* temp;
//drawObject* centerOrbit,center,firstOrbit,first,second;

class Scene{
public:
    Skybox* skybox;
    vector<OBJLoader> models;
    vector<drawObject*> baseObjects;
    vector<drawObject*> graph;
    std::vector<vec3> positions;

    Scene();
    
    void renderSkyBox(mat4,mat4);
    void addModel(const char*);
    void addBase(int m,vec3 offset,float angle ,  float scul,mat4 wrldMat,const char* texPath,const char* c,const char* a,const char* b, const char* vertPath,const char* fragPath);
    void render(Controller*);
    void addGraphNode(drawObject* dobj);
    void displayScene();
    vec3 getDifferenceVector();
    vector<drawObject*> getGraph();   
    void setChildMatrix(int tagNo,mat4 trs); 
};


Scene::Scene(){
    skybox = new Skybox();
    addModel("./models/beethoven.ply");//base mod
    addModel("./models/newsphere.ply");//base mod
    
    //addBase(vec3(0,0,0),0, 1,"./textures/earth.bmp","./textures/Disco_Ball_001_roughness.bmp","./textures/Disco_Ball_001_ambientOcclusion.bmp","./textures/Disco_Ball_001_normal.bmp","test.vs","test.fs");
    //LU
    //addBase(vec3(3,0,3),"./textures/Disco_Ball_001_basecolor.bmp","./textures/Disco_Ball_001_roughness.bmp","./textures/Disco_Ball_001_ambientOcclusion.bmp","./textures/Disco_Ball_001_normal.bmp","test.vs","test.fs");
    //RD
    //addBase(vec3(-3,-0,-3),"./textures/Disco_Ball_001_basecolor.bmp","./textures/Disco_Ball_001_roughness.bmp","./textures/Disco_Ball_001_normal.bmp","./textures/Disco_Ball_001_ambientOcclusion.bmp","test.vs","test.fs");
    //LD
    //addBase(vec3(3,-0,-3),"./textures/Colored_Pencils_001_basecolor.bmp","./textures/Colored_Pencils_001_roughness.bmp","./textures/Colored_Pencils_001_ambientOcclusion.bmp","./textures/Colored_Pencils_001_normal.bmp","test.vs","test.fs");
    //RU
    //addBase(vec3(-3,0,3),"./textures/Colored_Pencils_001_basecolor.bmp","./textures/Colored_Pencils_001_roughness.bmp","./textures/Colored_Pencils_001_normal.bmp","./textures/Colored_Pencils_001_ambientOcclusion.bmp","test.vs","test.fs");
    

    drawObject* centerOrbit = new drawObject(0,vec3(0,0,0),0, 1, mat4(1.0),"./textures/earth.bmp","./textures/Disco_Ball_001_roughness.bmp","./textures/Disco_Ball_001_ambientOcclusion.bmp","./textures/Disco_Ball_001_normal.bmp","test.vs","test.fs");
    drawObject* earth = new drawObject(0,vec3(0,0,0),0, 2, mat4(1.0),"./textures/earth.bmp","./textures/Disco_Ball_001_roughness.bmp","./textures/Disco_Ball_001_ambientOcclusion.bmp","./textures/Disco_Ball_001_normal.bmp","test.vs","test.fs");
    drawObject* trashcan = new drawObject(1,vec3(0,0,0),0, 1, mat4(1.0),"./textures/trashcan.bmp","./textures/Disco_Ball_001_roughness.bmp","./textures/Disco_Ball_001_ambientOcclusion.bmp","./textures/Disco_Ball_001_normal.bmp","test.vs","test.fs");
    drawObject* pokeball = new drawObject(3,vec3(7,7,0),0, 1, mat4(1.0),"./textures/pokeball.bmp","./textures/Disco_Ball_001_roughness.bmp","./textures/Disco_Ball_001_ambientOcclusion.bmp","./textures/Disco_Ball_001_normal.bmp","test.vs","test.fs");
    drawObject* orange = new drawObject(4,vec3(0,2,0),0, 1, mat4(1.0),"./textures/oranges.bmp","./textures/Disco_Ball_001_roughness.bmp","./textures/Disco_Ball_001_ambientOcclusion.bmp","./textures/Disco_Ball_001_normal.bmp","test.vs","test.fs");
    //drawObject* firstOrbit = new drawObject(2,vec3(5,0,0),0, 1, mat4(1.0),"./textures/pokeball.bmp","./textures/Disco_Ball_001_roughness.bmp","./textures/Disco_Ball_001_ambientOcclusion.bmp","./textures/Disco_Ball_001_normal.bmp","test.vs","test.fs");
    //drawObject* pokeball = new drawObject(2,vec3(0,0,0),0, 1, mat4(1.0),"./textures/pokeball.bmp","./textures/Disco_Ball_001_roughness.bmp","./textures/Disco_Ball_001_ambientOcclusion.bmp","./textures/Disco_Ball_001_normal.bmp","test.vs","test.fs");
    
    centerOrbit->appendChild(earth,centerOrbit);
    earth->appendChild(trashcan,earth);
    trashcan->appendChild(pokeball,trashcan);
    pokeball->appendChild(orange,pokeball);
    


    //firstOrbit->appendChild(pokeball,firstOrbit);

    addGraphNode(centerOrbit);
    //addGraphNode(firstOrbit);
    
}

void Scene::displayScene(){
    //std::vector<drawObject*> v = getGraph();
    graph[0]->setLocalMatrix(TRS(vec3(0,0*cos(glfwGetTime()),0),0,1));
    //center->setLocalMatrix(TRS(vec3(0,0,0),0, 1));
    //graph[1]->setLocalMatrix(TRS(vec3(5,5*cos(glfwGetTime()),0),0, 1));
    //first->setLocalMatrix(TRS(vec3(0,0,0),0, 1));
    //second->setLocalMatrix(TRS(vec3(5*cos(glfwGetTime()),0,5*cos(glfwGetTime())),0, 1));
    
    vec3 difference = getDifferenceVector();
    //cout << "HAHAHAH "<< difference.x << "," << difference.y << "," << difference.z << endl; 
    //graph[0]->children[0]->children[0]->children[0]->setDifferenceVector(difference);
    graph[0]->children[0]->children[0]->children[0]->setDifferenceVector(difference);
}

void Scene::setChildMatrix(int tagNo,mat4 trs){
    temp = graph[0];
    while(temp->children.size()){
            if(temp->getID()==tagNo) break;
            else temp = temp->children[0];
        
    }
    cout << "HEHRHER " << temp->getID() << endl;
    temp->setLocalMatrix(trs);
    printMatrixOnly(trs);



}

vec3 Scene::getDifferenceVector(){
    temp = graph[0];
    vec3 v1 = temp->getPosition();
    printVec3("1 ",v1);
    temp = temp->children[0];
    vec3 v2 = temp->getPosition();
    printVec3("2 ",v2);
    temp = temp->children[0];
    vec3 v3 = temp->getPosition();
    printVec3("3 ",v3);
    temp = temp->children[0];
    vec3 v4 = temp->getPosition();
    printVec3("4 ",v4);

    temp = temp->children[0];
    vec3 v5 = temp->getPosition();
    printVec3("5 ",v5);
    
    
    
    vec3 direction = v3 - v4;
    printVec3("new dir",direction);

    vec3 normalize_direction = normalize(direction);
    //printVec3("nor dir",normalize_direction);

    vec3 step = normalize_direction;

    cout << endl;
    return -step;
    //return vec3(0,0,0);
    
}


void Scene::renderSkyBox(mat4 p, mat4 v){
    Shader skyboxShader("./shaders/skybox.vs","./shaders/skybox.fs");
    //glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's ctrltent
    skyboxShader.use();
    glm::mat4 projection = p;
    glm::mat4 viewMatrix = v; // remove translation from the view matrix
    skyboxShader.setMat4("view", viewMatrix);
    skyboxShader.setMat4("projection", projection);
    // skybox cube
    glBindVertexArray(skybox->skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default   
}


void Scene::addModel(const char* path){
    OBJLoader obj;
    obj.loadOBJ(path);
    models.push_back(obj);
}
void Scene::addBase(int m,vec3 offset,float angle , float scul, mat4 wrldMat,const char* texPath,const char* c,const char* a,const char* b, const char* vertPath,const char* fragPath){
    drawObject* d = new drawObject(m,offset,angle,scul,wrldMat,texPath,c,a,b,vertPath,fragPath);
    baseObjects.push_back(d);
}

void Scene::addGraphNode(drawObject* dobj){
    graph.push_back(dobj);
}

vector<drawObject*> Scene::getGraph(){
    return graph;
}

void Scene::render(Controller* ctrl){    

    for(int i=0;i<graph.size();i++){

        //graph[i]->calcMatricesFromController(ctrl,models[0],"test.vs", "test.fs");
        graph[i]->calcMatrices(ctrl,models[0],"./shaders/test.vs", "./shaders/test.fs");
        //graph[i]->draw(models[0]);
        temp = graph[i];
        int flag = 1;
        while(temp->getNumChildren()){
            for(int i=0;i<temp->getNumChildren();i++){
                temp->children[i]->calcMatricesLOCALWORLD(ctrl,temp->worldMatrix,models[0],"./shaders/test.vs","./shaders/test.fs");
                temp->children[i]->draw(models[0]);
                temp = temp->children[i];
            }
            if(flag == 1) flag=0;
            
        }












        /*if(graph[i]->children.size() > 0){
            for(int j=0;j<graph[i]->children.size();j++){
                graph[i]->children[j]->calcMatricesLOCALWORLD(ctrl,graph[i]->worldMatrix,models[0],"test.vs", "test.fs");
                graph[i]->children[j]->draw(models[0]);
                //cout << "Children = " << graph[i]->children.size() << endl;         
            }
        }*/
    }

}








//model.udate
// for chidlern 
        // child.update






















class SceneGraphNode {
    public:
        OBJLoader* object;
        vector <SceneGraphNode*> edges;
        mat4 trans_rot_scale;
        SceneGraphNode(OBJLoader*);
        SceneGraphNode(OBJLoader*, mat4);
        SceneGraphNode* getNode(int i);
        void appendModelNode(SceneGraphNode* sm);
        void drawNodeList();
        void getPosition(GLfloat *pos);
        void drawModel();
        void setTransitions();
};

SceneGraphNode::SceneGraphNode(OBJLoader* o) {
    object = o;
    trans_rot_scale = TRS(vec3(0,0,0),0,1);
}  

SceneGraphNode::SceneGraphNode(OBJLoader* o, mat4 mat) {
    object = o;
    trans_rot_scale = mat;
}

void SceneGraphNode::appendModelNode(SceneGraphNode* sm) {
    edges.push_back(sm);
}

