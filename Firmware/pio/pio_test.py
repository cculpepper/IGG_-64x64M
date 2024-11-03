

import array, time
from machine import Pin
import rp2


NUM_LEDS = 8

D1_PIN = 0
NUM_DAT_PINS = 8
SHCP_PIN = 10
EN_5V_PIN = 11
TX_LED_PIN = 12

K_RST_PIN = 14
K_CLK_PIN = 15
SR_MC_PIN = 16
SR_STCP_PIN = 17
SR_OE_PIN = 18
HV_EN_PIN = 25

HV_V_ADC_PIN = 0
BAT_V_PIN = 1
HV_EN = Pin(HV_EN_PIN, Pin.OUT)
print("Turning on HV")
HV_EN.value(1)
time.sleep_ms(100)

print("Turning off HV")
HV_EN.value(0)
time.sleep_ms(100)
print("Turning on HV")
HV_EN.value(1)
time.sleep_ms(100)
K_RST_BIT = 0x1 # Inverted!
K_CLK_BIT = 0x2 # Inverted!
SR_MC_BIT = 0x4
SR_STCP_BIT = 0x8
SR_OE_BIT = 0x10

#K_RST goes to an or gate to the reset singnal
#The 4017s need a high on the reset, and increment with a going high pulse on the clock
RESET_KATHODE_BIT_A = K_CLK_BIT
RESET_KATHODE_BIT_B = K_RST_BIT + K_CLK_BIT 

RESET_ANODE_BIT_A = 0
RESET_ANODE_BIT_B = SR_MC_BIT + SR_STCP_BIT
RESET_STATE_A = RESET_KATHODE_BIT_A + RESET_ANODE_BIT_A
RESET_STATE_B = RESET_KATHODE_BIT_B + RESET_ANODE_BIT_B

#The anodes are 74AHCT595
# MR being to zero should reset all the anode drivers
# SR_STCP clocks over the SR outputs into the outputs on a positive pulse
# SHCP clocks the input data over on a positive pulse
# SR_OE enables the outputs, otherwise are in high-Z

CLOCKING_IN_DATA_STATE = SR_MC_BIT + K_RST_BIT + K_CLK_BIT

DATA_DONE_CLOCKING_SEND_OUT_A = SR_MC_BIT + SR_OE_BIT + K_RST_BIT

#Keep from reset, clock the shift_register to the output
DATA_DONE_CLOCKING_SEND_OUT_B = SR_MC_BIT + SR_STCP_BIT + SR_OE_BIT + K_RST_BIT

DATA_DONE_CLOCKING_SEND_OUT_C = SR_MC_BIT + SR_STCP_BIT  + K_RST_BIT





@rp2.asm_pio(sideset_init=rp2.PIO.OUT_LOW, out_init=[rp2.PIO.OUT_LOW,] * NUM_DAT_PINS, set_init = [rp2.PIO.OUT_LOW, ] * 5, out_shiftdir=rp2.PIO.SHIFT_LEFT, autopull=True, pull_thresh=32)
def iggx_pio():
    K_RST_BIT = 0x1
    K_CLK_BIT = 0x2
    SR_MC_BIT = 0x4
    SR_STCP_BIT = 0x8
    SR_OE_BIT = 0x10


    DONT_RESET_KATHODE = K_RST_BIT # Inverter on this pin
    DONT_CLOCK_KATHODE = K_CLK_BIT # Keep the clock high (really a low to the chip)
    CLOCK_KATHODE = 0 # Send the clock low (really a high to the chip
    
    DONT_RESET_ANODE_SR = SR_MC_BIT
    RESET_ANODE_SR = 0
    
    #K_RST goes to an or gate to the reset singnal
    #The 4017s need a high on the reset, and increment with a going high pulse on the clock
    RESET_KATHODE_BIT_A = DONT_RESET_KATHODE  + DONT_CLOCK_KATHODE
    RESET_KATHODE_BIT_B = DONT_CLOCK_KATHODE
    

    RESET_ANODE_BIT_A = RESET_ANODE_SR
    RESET_ANODE_BIT_B = DONT_RESET_ANODE_SR + SR_STCP_BIT
    
    RESET_STATE_A = RESET_KATHODE_BIT_A + RESET_ANODE_BIT_A + SR_OE_BIT
    RESET_STATE_B = RESET_KATHODE_BIT_B + RESET_ANODE_BIT_B + SR_OE_BIT

    #The anodes are 74AHCT595
    # MR being to zero should reset all the anode drivers
    # SR_STCP clocks over the SR outputs into the outputs on a positive pulse
    # SHCP clocks the input data over on a positive pulse
    # SR_OE enables the outputs, otherwise are in high-Z

    CLOCKING_IN_DATA_STATE = DONT_RESET_ANODE_SR + DONT_RESET_KATHODE + DONT_CLOCK_KATHODE

    # Don't reset the K, turn off output, clock the shift to output, and clock the Kathode
    DATA_DONE_CLOCKING_SEND_OUT_A = DONT_RESET_KATHODE + SR_OE_BIT + DONT_RESET_ANODE_SR

    #Don't reset K, Keep the shift clocking to output, but enable output
    DATA_DONE_CLOCKING_SEND_OUT_B = DONT_RESET_KATHODE + CLOCK_KATHODE + SR_OE_BIT + DONT_RESET_ANODE_SR + SR_STCP_BIT

    DATA_DONE_CLOCKING_SEND_OUT_C = DONT_RESET_KATHODE + DONT_RESET_ANODE_SR + DONT_CLOCK_KATHODE
    
    DATA_DONE_CLOCKING_SEND_OUT_D = DONT_RESET_KATHODE + DONT_RESET_ANODE_SR + DONT_CLOCK_KATHODE
    
    # 0x01 is KRST, 02 is KCLK, 04 is MC, 08 is STCP
    #0x01:  Just KCLK 2: Just KRST, 4: Both KCLK and KRST, 8: Same as 4, 10: K_CLK, KRST, OE, Side: KCLK, KRST, SHCP
    
    set(pins, RESET_STATE_A) .side(0) [7]
    set(pins, RESET_STATE_B) .side(1) [7]
    wrap_target()
    set(x, 8) .side(0)  # TODO Check this
    set(pins, CLOCKING_IN_DATA_STATE) .side(0)
    label("clock_out")
    out(pins, 8) .side(0)
    nop() .side(1) # side set here
    nop() .side(0)
    jmp(x_dec,"clock_out") .side(0)

    label("output")
    set(pins, DATA_DONE_CLOCKING_SEND_OUT_A) .side(0) [7]
    set(pins, DATA_DONE_CLOCKING_SEND_OUT_B) .side(0) [7]
    set(pins, DATA_DONE_CLOCKING_SEND_OUT_C) .side(0) [7]
    set(pins, DATA_DONE_CLOCKING_SEND_OUT_D) .side(0) [7]
    nop() .side(0) [7]

    wrap()


# Create the StateMachine with the ws2812 program, outputting on Pin(22).
#sm = rp2.StateMachine(0, iggx_pio, freq=8_000_000, sideset_base=Pin(SHCP_PIN), out_base=Pin(D1_PIN), set_base=Pin(K_RST_PIN), pull_thresh=32)
sm = rp2.StateMachine(0, iggx_pio, freq=1_000_000, sideset_base=Pin(SHCP_PIN), out_base=Pin(D1_PIN), set_base=Pin(K_RST_PIN), pull_thresh=32)

# Start the StateMachine, it will wait for data on its FIFO.
sm.active(1)

# Display a pattern on the LEDs via an array of LED RGB values.
ar = array.array("I", [0 for _ in range(NUM_LEDS)])

a = [0] * 8
a = 0
while True:
    for i in range(64):
        sm.put(i, 8)
    #time.sleep_ms(1)

# Fade out.
for i in range(24):
    for j in range(NUM_LEDS):
        ar[j] >>= 1
    sm.put(ar, 8)
    time.sleep_ms(50)
