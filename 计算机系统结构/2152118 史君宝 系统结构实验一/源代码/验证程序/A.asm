# Set initial durability threshold for survivable falls
addi $9, $0, 60
# Set initial maximum floor height
addi $10, $0, 100
# Initialize counters for statistics
addi $24, $0, 0  # Total attempts
addi $25, $0, 0  # Broken eggs
# Initialize loop variables
addi $1, $0, 1
add $2, $10, $0
# Loop to determine maximum survivable floor height
initialize_loop:
  # Jump to main loop
  j main_loop

# Main loop
main_loop:
  # Calculate midpoint between $1 and $2
  add $4, $1, $2
  sra $3, $4, 1
  # Check if midpoint is less than durability threshold
  slt $5, $3, $9
  bne $5, $0, lessequ
  # Egg breaks (midpoint > threshold)
  addi $24, $24, 1  # Increment total attempts
  addi $25, $25, 1  # Increment broken eggs
  addi $2, $3, -1   # Adjust maximum floor height
  # Check if $1 < $2
  slt $5, $1, $2
  bne $5, $0, main_loop
  # Reset $1 to 1 and restart the loop
  addi $1, $0, 1
  j initialize_loop

# Successful attempt (midpoint <= threshold)
lessequ:
  addi $24, $24, 1  # Increment total attempts
  # Check if $1 == $3
  bne $1, $3, repeat
  addi $3, $3, 1    # Increment $3 if $1 != $3

# Repeat loop for the next floor
repeat:
  addi $1, $3, 0
  j initialize_loop

# End of program