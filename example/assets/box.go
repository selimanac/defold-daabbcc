embedded_components {
  id: "sprite"
  type: "sprite"
  data: "default_animation: \"40x40_rect\"\n"
  "material: \"/builtins/materials/sprite.material\"\n"
  "textures {\n"
  "  sampler: \"texture_sampler\"\n"
  "  texture: \"/example/assets/main.atlas\"\n"
  "}\n"
  ""
}
embedded_components {
  id: "label"
  type: "label"
  data: "size {\n"
  "  x: 128.0\n"
  "  y: 32.0\n"
  "}\n"
  "text: \"9\\\\n"
  "\"\n"
  "  \"\\n"
  "\"\n"
  "  \"10\"\n"
  "font: \"/builtins/fonts/debug/always_on_top.font\"\n"
  "material: \"/builtins/fonts/label.material\"\n"
  ""
  position {
    z: 0.1
  }
}
