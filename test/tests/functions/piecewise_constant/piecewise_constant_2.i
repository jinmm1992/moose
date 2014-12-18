[Mesh]
  type = GeneratedMesh
  dim = 2
[]

[Variables]
  active = 'u'

  [./u]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Functions]
  [./bc_func]
    type = PiecewiseLinear
    x = '0.0 1.0'
    y = '10 100.0'
    axis = 1
  [../]
 # [./bc_func]
 #   type = ParsedFunction
 #   value = (100-10)*y+10
 # [../]
[]

[Kernels]
  active = 'diff'

  [./diff]
    type = Diffusion
    variable = u
  [../]
[]

[BCs]
  active = 'left right'

  [./left]
    type = DirichletBC
    variable = u
    boundary = left
    value = 0
  [../]

  [./right]
    type = FunctionDirichletBC
    variable = u
    boundary = right
    function = bc_func
  [../]
[]

[Executioner]
  type = Steady

  solve_type = 'NEWTON'
[]

[Outputs]
  output_initial = true
  exodus = true
  [./console]
    type = Console
    perf_log = true
    linear_residuals = true
  [../]
[]
