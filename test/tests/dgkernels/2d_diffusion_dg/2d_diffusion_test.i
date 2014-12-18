[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
#  xmin = -1
#  xmax = 1
#  ymin = -1
#  ymax = 1
  xmin = 0
  xmax = 1
  ymin = 0
  ymax = 1
 # elem_type = QUAD4
[]

[Variables]
  active = 'u'

  [./u]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = u
  [../]
[]

[DGKernels]
  active = 'dg_diff'

  [./dg_diff]
    type = DGDiffusion
    variable = u
    epsilon = -1
    sigma = 6
  [../]
[]

[Functions]
  [./bcfunc]
    type = ParsedFunction
    value = 2*y+2*x
  [../]
[]

[BCs]
  active = 'left right'

  [./right]
    type = DGFunctionDiffusionDirichletBC
    variable = u
    boundary = 1
    function = bcfunc
    epsilon = -1
    sigma = 6
  [../]

  [./left]
    type = DirichletBC
    variable = u
    boundary = 3
    value = 1
  [../]
[]

[Executioner]
  type = Steady

  solve_type = 'NEWTON'
[]

[Outputs]
  file_base = simple_diffusion
  output_initial = true
  exodus = true
  [./console]
    type = Console
    perf_log = true
    linear_residuals = true
  [../]
[]
