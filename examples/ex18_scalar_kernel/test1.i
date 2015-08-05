#[GlobalParams]
#   use_displaced_mesh = true
#[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
  xmin = 1
  xmax = 2
  displacements = 'disp_x disp_y'
[]

[Variables]
  [./u]
  [../]
  # ODE variables
  [./x]
    family = SCALAR
    order = FIRST
    initial_condition = 1
  [../]
  [./y]
    family = SCALAR
    order = FIRST
    initial_condition = 2
  [../]

[]

[AuxVariables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./stretch]
  [../]
[]

[Functions]
  [./stretch_func]
    type = ParsedFunction
    value = a #t
    vars = a
  [../]
[]

[Kernels]
  [./td]
    type = TimeDerivative
    variable = u
    use_displaced_mesh = true
  [../]

  [./diff]
    type = Diffusion
    variable = u
    use_displaced_mesh = true
  [../]
[]

[ScalarKernels]
  [./td1]
    type = ODETimeDerivative
    variable = x
  [../]
  [./ode1]
    type = ImplicitODEx
    variable = x
    y = y
    pp = avg
    execute_on = timestep_end
  [../]

  [./td2]
    type = ODETimeDerivative
    variable = y
  [../]
  [./ode2]
    type = ImplicitODEy
    variable = y
    x = x
  [../]
[]
[AuxKernels]
  [./interpolation]
    type = CoupledDirectionalMeshHeightInterpolation
    variable = disp_x
    direction = x
    execute_on = timestep_begin
    coupled_var = stretch
  [../]
  [./stretch_aux]
    type = FunctionAux
    variable = stretch
    function = stretch_func
    execute_on = timestep_begin
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = u
    boundary = left
    value = 0
    use_displaced_mesh = true
  [../]
  [./right]
    type = NeumannBC
    variable = u
    boundary = right
    value = 1
    use_displaced_mesh = true
  [../]
[]

[Postprocessors]
  [./avg]
  type = AverageNodalVariableValue
  variable = u
  [../]
  # to print the values of x, y into a file so we can plot it
  [./x]
    type = ScalarVariable
    variable = x
    execute_on = timestep_end
  [../]
  [./y]
    type = ScalarVariable
    variable = y
    execute_on = timestep_end
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 5
  dt = 0.1

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  output_initial = true
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
