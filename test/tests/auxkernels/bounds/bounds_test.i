[Mesh]
  type = GeneratedMesh

  dim = 2

  xmin = 0
  xmax = 1

  ymin = 0
  ymax = 1

  nx = 10
  ny = 10
[]

[Variables]
  [./u]
    order = FIRST
    family = LAGRANGE
  [../]

  [./v]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxVariables]
  [./bounds_dummy]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./diff_u]
    type = Diffusion
    variable = u
  [../]

  [./diff_v]
    type = Diffusion
    variable = v
  [../]
[]

[BCs]
  [./left_u]
    type = DirichletBC
    variable = u
    boundary = 3
    value = 0
  [../]

  [./right_u]
    type = DirichletBC
    variable = u
    boundary = 1
    value = 10
  [../]

  [./left_v]
    type = DirichletBC
    variable = v
    boundary = 3
    value = 0
  [../]

  [./right_v]
    type = DirichletBC
    variable = v
    boundary = 1
    value = 5
  [../]
[]

[Bounds]
  [./u_bounds]
    type = BoundsAux
    variable = bounds_dummy
    bounded_variable = u
    upper = 1
    lower = 0
  [../]

  [./v_bounds]
    type = BoundsAux
    variable = bounds_dummy
    bounded_variable = v
    upper = 3
    lower = -1
  [../]
[]

[Executioner]
  type = Steady

  # Preconditioned JFNK (default)
  solve_type = 'NEWTON'
  petsc_options_iname = '-snes_type'
  petsc_options_value = 'vinewtonssls'
[]

[Outputs]
  file_base = out
  output_initial = true
  exodus = true
  [./console]
    type = Console
    perf_log = true
  [../]
[]
