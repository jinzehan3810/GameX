#include "GameX/renderer/object.h"

#include "GameX/application/application.h"
#include "GameX/renderer/renderer.h"

namespace GameX {

Object::Object(Renderer *renderer) : renderer_(renderer) {
}

StaticObject::StaticObject(Renderer *renderer, const Mesh &mesh)
    : Object(renderer) {
  vertex_buffer_.Init(renderer_->App()->Core(), mesh.Vertices().size());
  vertex_buffer_.UploadContents(mesh.Vertices().data(), mesh.Vertices().size());
  index_buffer_.Init(renderer_->App()->Core(), mesh.Indices().size());
  index_buffer_.UploadContents(mesh.Indices().data(), mesh.Indices().size());
}

DynamicObject::DynamicObject(Renderer *renderer, const Mesh *mesh)
    : Object(renderer), mesh_(mesh) {
  vertex_buffer_.Init(renderer_->App()->Core(), mesh->Vertices().size());
  vertex_buffer_.UploadContents(mesh->Vertices().data(),
                                mesh->Vertices().size());
  index_buffer_.Init(renderer_->App()->Core(), mesh->Indices().size());
  index_buffer_.UploadContents(mesh->Indices().data(), mesh->Indices().size());

  renderer_->RegisterSyncObject(&vertex_buffer_);
}

DynamicObject::~DynamicObject() {
  renderer_->UnregisterSyncObject(&vertex_buffer_);
}

bool DynamicObject::SyncData(VkCommandBuffer cmd_buffer) {
  ;
  return SyncData(cmd_buffer, renderer_->App()->Core()->CurrentFrame());
}

bool DynamicObject::SyncData(std::function<void(VkCommandBuffer)> &func) {
  return SyncData(func, renderer_->App()->Core()->CurrentFrame());
}

bool DynamicObject::SyncData(VkCommandBuffer cmd_buffer, uint32_t frame_index) {
  return vertex_buffer_.SyncData(cmd_buffer, frame_index);
}

bool DynamicObject::SyncData(std::function<void(VkCommandBuffer)> &func,
                             uint32_t frame_index) {
  return vertex_buffer_.SyncData(func, frame_index);
}
}  // namespace GameX