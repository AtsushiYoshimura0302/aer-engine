// -----------------------------------------------------------------------------
// CreativeCommons BY-SA 3.0 2013 <Thibault Coppex>
//
//
// -----------------------------------------------------------------------------

#include "aura/character.h"

#include "aer/animation/blend_tree.h"
#include "aer/animation/blend_node.h"
#include "aer/loader/skma.h"
#include "aer/loader/skma_utils.h"
#include "aer/view/camera.h"


void Character::init() {
  std::string filename = DATA_DIRECTORY "models/sintel/sintel";
  AER_CHECK(skmModel_.load(filename + ".skm", filename + ".mat")); //

  // ------------

  // Program shaders
  init_shaders();

  // Statically set the animation properties used by the character.
  // Future version should use an Action State Machine load at runtime with AI.
  init_animations();

  // Same with blend shape animation datas
  init_blendshapes();
}

void Character::update() {
  //-------------------------------------
#if 1
  //-- (rarely done manually, and not this way. Prefer to use a blendtree)
  float t = 0.5f * aer::GlobalClock::Get().application_time(aer::SECOND);
  float s = 0.5f * (1.0f + cos(5.0f*t));
  s = glm::smoothstep(0.0f, 1.0f, s);


  aer::BlendTree &morph_blendtree = skmModel_.morph_controller().blend_tree();
  aer::CoeffNode *coeffnode = morph_blendtree.find_node<aer::CoeffNode>("face_coeff");
  AER_CHECK(nullptr != coeffnode);
  coeffnode->set_factor(s);
#endif
  //-------------------------------------

  //---

  skmModel_.update(); //
}

void Character::render(const aer::Camera &camera) {  
  aer::I32 texUnit = 0;

  mProgram.activate();

  // --- Skinning on GPU using GLSL ------------------------
  skmModel_.skeleton_controller().bind_skinning_texture(texUnit);
  mProgram.set_uniform("uSkinningDatas", texUnit);
  ++texUnit;
  
  CHECKGLERROR();
  // -------------------------------------------------------

  // --- Blend Shapes --------------------------------------
  aer::BlendShape &blendshape = skmModel_.morph_controller().blend_shape();

  blendshape.bind_texture_buffer(aer::BlendShape::BS_INDICES, texUnit);
  mProgram.set_uniform("uBS_indices", texUnit);
  ++texUnit;

  blendshape.bind_texture_buffer(aer::BlendShape::BS_WEIGHTS, texUnit);
  mProgram.set_uniform("uBS_weights", texUnit);
  ++texUnit;

  blendshape.bind_texture_buffer(aer::BlendShape::BS_LUT, texUnit);
  mProgram.set_uniform("uBS_LUT", texUnit);
  ++texUnit;

  blendshape.bind_texture_buffer(aer::BlendShape::BS_DATAS, texUnit);
  mProgram.set_uniform("uBS_data", texUnit);
  ++texUnit;

  mProgram.set_uniform("uNumBlendShape", blendshape.count());
  mProgram.set_uniform("uUsedBlendShape", 
                       skmModel_.morph_controller().total_expressions());

  CHECKGLERROR();
  // -------------------------------------------------------


  // --- Rendering -----------------------------------------
  // TODO : use custom sampler
  aer::DefaultSampler::AnisotropyRepeat().bind(texUnit);

  const aer::Matrix4x4 &viewProj = camera.view_projection_matrix();
  mProgram.set_uniform("uModelViewProjMatrix", viewProj);

  const aer::Mesh &mesh = skmModel_.mesh(); //
  for (aer::U32 i = 0u; i < mesh.submesh_count(); ++i) {
    aer::I32 material_id = mesh.submesh_tag(i);

    bool bEnableTexturing = false;
    if (material_id >= 0) {
      const auto &material = skmModel_.material(material_id); //

      if (material.has_diffuse_map()) {
        bEnableTexturing = true;
        mProgram.set_uniform("uDiffuseMap", texUnit);
        material.diffuse_map()->bind(texUnit);
      } else {
        mProgram.set_uniform("uDiffuseColor", material.phong_attributes().diffuse);
      }
    }
    mProgram.set_uniform("uEnableTexturing", bEnableTexturing);

    mesh.draw_submesh(i);
  }
  // -------------------------------------------------------

  mProgram.deactivate();
  CHECKGLERROR();
}

void Character::init_shaders() {
  aer::ShaderProxy &sp = aer::ShaderProxy::Get();

  // used by rendering, skinning and blend shapes
  mProgram.create();
  mProgram.add_shader(sp.get("Skinning.VS"));
  mProgram.add_shader(sp.get("Skinning.FS"));
  AER_CHECK(mProgram.link());

  CHECKGLERROR();
}

void Character::init_animations() {
  aer::BlendTree &blendtree = skmModel_.skeleton_controller().blend_tree();

  aer::LeaveNode *leave1 = blendtree.add_leave("LeftArmDown");
#if 0
  LeaveNode *leave2 = blendtree.add_leave("RightArmDown");
  LerpNode *node = new LerpNode(leave1, leave2);
  node->set_factor(0.5f);
  blendtree.add_node("Lerp_test", node);
#endif
}

void Character::init_blendshapes() {
  //-------------------------------------
  aer::Expression_t expression;
  std::string expr_name = "facial_stuff";
  expression.pName = new char[64];
  sprintf(expression.pName, "%s", expr_name.c_str());

  expression.clip_duration = 1.0f;
  expression.bLoop         = true;
  expression.bManualBypass = true; // don't use time to compute expressions
  expression.indices = {55, 56, 43, 3};
  //expression.bPingPong = true;

  skmModel_.morph_controller().add_expressions(&expression, 1u);

  //-------------------------------------

  aer::BlendTree &blendtree = skmModel_.morph_controller().blend_tree();
  aer::LeaveNode *leave1 = blendtree.add_leave(expr_name);
  blendtree.add_node("face_coeff", new aer::CoeffNode(leave1));
}
