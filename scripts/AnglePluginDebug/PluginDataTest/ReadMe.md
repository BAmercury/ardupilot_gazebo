## Data:

See https://github.com/BAmercury/ardupilot_gazebo/issues/50 for progress and final conclusions on the testing process

# Trial 1

Doublet manuever on the Gazebo Y axis:
    - Started by moving the drone to the right (Negative on the Gazebo Y axis), then brought to the left side
      sometimes stopping at the origin and waiting till trim

    - working_data.csv
        - AnglePlugin Data for normalized distance vector
    - working_IR_data.csv
        - IRLock Data

# Trial 2:

Same doublet manuever as trial 1. This trial was to test to see if the assumption is correct that IRLOCK X data is the Gazebo AnglePlugin Y data but negated ( "-AnglePlugin.Y = Irlock.X" )

    - AnglePlugin_Y_2.csv
        - This time the correctedPos.Y data is negated (-correctedPos.Y) since it was noted from previous trial that
          the IRLock data for the Y axis happened to be the X data but invereted

# Trial 3

Doublet manuever on the X axis for both plugins. Manuever is forward, then back (In gazebo world frame coords)

    - AnglePlugin_X.csv
        - First set of data for a doublet manuever on the X axis via AnglePlugin
    - IR_data_X.csv
        - First set of data for the doublet manuever for the Y axis of AnglePlugin

What I learned here is that AnglePlugin.X = IRLock_data.Y


# Trial 4

Ran both Angle and the IR data plugins at the same time and logged. This shows the final results of my transformations

    - data.csv
        - AnglePlugin Data
    - IR_data.csv
        - IRlock data