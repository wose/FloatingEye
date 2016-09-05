#include "BodyWidget.h"
#include "Primitives.h"

#include "IrrIMGUI/IrrIMGUI.h"
#include "IrrIMGUI/IncludeIrrlicht.h"

#include <string>

using namespace irr;
using namespace irr::core;
using namespace irr::video;
using namespace irr::scene;

BodyWidget::BodyWidget(const std::string& name, irr::IrrlichtDevice* const irrDevice,
                       IrrIMGUI::IIMGUIHandle* const imGuiHandle) :
    name_{name},
    irrDevice_{irrDevice},
    imGuiHandle_{imGuiHandle}
{
    auto driver = irrDevice->getVideoDriver();
    auto sceneManager = irrDevice->getSceneManager();
    sceneNode_ = sceneManager->addEmptySceneNode();
    cameraNode_ = sceneManager->addCameraSceneNode(sceneNode_, vector3df(0, 2, 10), vector3df(0, 0, 0));
    cameraNode_->setAspectRatio(1.0);

    renderTarget_ = driver->addRenderTargetTexture(core::dimension2d<u32>(512, 512), name_.c_str());
    renderTargetID_ = imGuiHandle->createTexture(renderTarget_);

    sceneManager->setAmbientLight(SColorf(0.3f, 0.3f, 0.3f));
    auto lightNode = sceneManager->addLightSceneNode(sceneNode_,
                                                     vector3df(30, 0, 0),
                                                     SColorf(1.0f, 1.0f, 1.0f),
                                                     100.0f);

    meshTexture_ = driver->getTexture("res/Phobos.jpg");

    meshNode_ = sceneManager->addSphereSceneNode(5.0, 64, sceneNode_);
    meshNode_->setPosition(vector3df(0.0, 0.0, 0.0));
    meshNode_->setMaterialTexture(0, meshTexture_);
    meshNode_->setMaterialFlag(EMF_LIGHTING, true);

    edgeTexture_ = createColorTexture(driver, SColor(255, 0, 255, 255));
    markerMaterial_.Lighting = false;
    markerMaterial_.setTexture(0, edgeTexture_);

    auto xyPlane = createCircle(64, 6.0, SColor(255, 255, 255, 255));
    xyPlane->recalculateBoundingBox();
    xyPlane->setHardwareMappingHint(EHM_STATIC);
    meshBuffers_.push_back(xyPlane);
}

BodyWidget::~BodyWidget()
{
    for (auto meshBuffer : meshBuffers_) {
        meshBuffer->drop();
    }

    imGuiHandle_->deleteTexture(renderTargetID_);
}

void BodyWidget::render()
{
    auto sceneManager = irrDevice_->getSceneManager();
    auto driver = irrDevice_->getVideoDriver();

    driver->setRenderTarget(renderTarget_, true, true, video::SColor(255, 26, 26, 26));
    sceneNode_->setVisible(true);
    sceneManager->setActiveCamera(cameraNode_);

    meshNode_->setVisible(false);
    if (showTexture_) {
        meshNode_->setMaterialFlag(EMF_LIGHTING, true);
        meshNode_->setMaterialTexture(0, meshTexture_);
        meshNode_->setMaterialFlag(video::EMF_POINTCLOUD, false);
        meshNode_->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
        meshNode_->setVisible(true);
    }

    if (showPointCloud_) {
        sceneManager->drawAll();
        meshNode_->setScale(vector3df(1.05, 1.05, 1.05));
        meshNode_->setMaterialFlag(EMF_LIGHTING, false);
        meshNode_->setMaterialTexture(0, edgeTexture_);
        meshNode_->setMaterialFlag(video::EMF_POINTCLOUD, true);
        meshNode_->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
        meshNode_->setVisible(true);
    }

    sceneManager->drawAll();
    meshNode_->setScale(vector3df(1, 1, 1));

    driver->setMaterial(markerMaterial_);
    for (auto meshBuffer : meshBuffers_) {
        driver->drawVertexPrimitiveList((S3DVertex*)meshBuffer->getVertices(),
                                        meshBuffer->getVertexCount(),
                                        &meshBuffer->getIndices()[0],
                                        meshBuffer->getIndexCount(),
                                        video::EVT_STANDARD,
                                        scene::EPT_LINE_LOOP);
    }

    sceneNode_->setVisible(false);
}

void BodyWidget::draw()
{
    imGuiHandle_->updateTexture(renderTargetID_, renderTarget_);

    /*    ImGui::Begin(name_.c_str(), NULL,
                 ImGuiWindowFlags_NoTitleBar
                 | ImGuiWindowFlags_NoResize
                 | ImGuiWindowFlags_NoMove
                 | ImGuiWindowFlags_NoBringToFrontOnFocus
                 | ImGuiWindowFlags_NoScrollbar);
    */
    ImGui::Begin(name_.c_str());
    ImGui::Checkbox("Texture", &showTexture_);
    ImGui::Checkbox("Point Cloud", &showPointCloud_);
    ImGui::Image(*renderTargetID_, ImVec2(512, 512));
    ImGui::End();
}
