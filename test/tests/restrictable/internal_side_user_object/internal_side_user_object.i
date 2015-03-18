[Mesh]
  type = FileMesh
  file = rectangle.e
  dim = 2
[]

[Variables]
  [./u]
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = u
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = u
    boundary = 1
    value = 0
  [../]
  [./right]
    type = DirichletBC
    variable = u
    boundary = 2
    value = 1
  [../]
[]

[Postprocessors]
  [./all_pp]
    type = NumInternalSides
   [../]
  [./block_1_pp]
    type = NumInternalSides
    block = 1
  [../]
  [./block_2_pp]
    type = NumInternalSides
    block = 2
  [../]
[]

[Executioner]
  type = Steady
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  output_initial = true
  exodus = false
  csv = true
  print_linear_residuals = true
  print_perf_log = true
[]
