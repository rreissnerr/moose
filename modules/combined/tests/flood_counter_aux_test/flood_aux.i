[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 50
  ny = 50
  nz = 0

  xmax = 40
  ymax = 40
  zmax = 0
  elem_type = QUAD4
[]

[Variables]
  [./u]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxVariables]
  [./bubble_map]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  active = 'diff forcing_1 forcing_2 forcing_3 forcing_4 dot'

  [./diff]
    type = Diffusion
    variable = u
  [../]

  [./forcing_1]
    type = GaussContForcing
    variable = u
    x_center = 1.0
    y_center = 1.0
    x_spread = 0.5
    y_spread = 0.5
  [../]

  [./forcing_2]
    type = GaussContForcing
    variable = u
    x_center = 20.0
    y_center = 39.0
    x_spread = 0.5
    y_spread = 0.5
  [../]

  [./forcing_3]
    type = GaussContForcing
    variable = u
    x_center = 39.0
    y_center = 20.0
    x_spread = 0.5
    y_spread = 0.5
  [../]

  [./forcing_4]
    type = GaussContForcing
    variable = u
    x_center = 15.0
    y_center = 15.0
    x_spread = 0.5
    y_spread = 0.5
  [../]

  [./dot]
    type = TimeDerivative
    variable = u
  [../]
[]

[AuxKernels]
  [./mapper]
    type = FeatureFloodCountAux
    variable = bubble_map
    execute_on = timestep_end
    bubble_object = bubbles
  [../]
[]

[BCs]
  [./Periodic]
    [./x]
      variable = u
      auto_direction = 'x y'
    [../]
  [../]
[]

[UserObjects]
  [./bubbles]
    type = FeatureFloodCount
    variable = u
    threshold = 0.3
    execute_on = timestep_end
  [../]
[]

[Executioner]
  active = ''

  type = Transient
  dt = 4.0
  num_steps = 5

  [./Adaptivity]
    refine_fraction = .40
    coarsen_fraction = .02
    max_h_level = 3
    error_estimator = KellyErrorEstimator
  [../]
[]

[Outputs]
  file_base = out
  exodus = true
  [./console]
    type = Console
    perf_log = true
    output_on = 'failed nonlinear linear timestep_end'
  [../]
[]
