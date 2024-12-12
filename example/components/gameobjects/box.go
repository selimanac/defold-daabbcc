embedded_components {
  id: "sprite"
  type: "sprite"
  data: "default_animation: \"player_40x40\"\n"
  "material: \"/builtins/materials/sprite.material\"\n"
  "textures {\n"
  "  sampler: \"texture_sampler\"\n"
  "  texture: \"/example/components/atlas/test.atlas\"\n"
  "}\n"
  ""
}
embedded_components {
  id: "label"
  type: "label"
  data: "size {\n"
  "  x: 40.0\n"
  "  y: 32.0\n"
  "}\n"
  "text: \"1\"\n"
  "font: \"/builtins/fonts/default.font\"\n"
  "material: \"/builtins/fonts/label-df.material\"\n"
  ""
  position {
    z: 0.01
  }
}
