/**
 * @ingroup   lib_io68
 * @file      io68/mfpemul.h
 * @brief     MFP-68901 emulator header.
 * @author    Benjamin Gerard
 * @date      1999/03/20
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef IO68_MFPEMUL_H
#define IO68_MFPEMUL_H

#include "io68_api.h"
#include "emu68/struct68.h"

/**
 * @defgroup  lib_io68_mfp  MFP-68901 emulator
 * @ingroup   lib_io68
 * @brief     MFP-68901 (Atari-ST timers) emulator.
 *
 *  Motorola Multi Function Peripheral is a multi purpose IO chip:
 * - 8 bit parallele port.
 * - Each bit has indepedant direction
 * - Each bit can be a interruption source
 * - 16 interruption sources
 * - 4 universal timers (most part of this emulator)
 * - Integrated serial interface.
 * - 24 registers
 *
 * @p Registers
 *
 * - 00 @b GPIP (General Purpose I/O Interrupt port):
 *
 *   Data Register for the 8 bit port for reading or writing.
 *   - bit#0 : Centronics (parrallele port)
 *   - bit#1 : Rs232 carrier detection
 *   - bit#1 : Rs232 CTS (clear to send)
 *   - bit#3 : Blitter
 *   - bit#4 : MIDI/Keyboard irq
 *   - bit#5 : FDC/DMA interrupt
 *   - bit#6 : Rs232 ring
 *   - bit#7 : Monochorm monitor detect
 *
 * - 01 @b AER (Active Edge Register):
 *
 *   If port's bit are an interruption source this register selects
 *   interruption direction (0:descending 1:ascending).
 *
 *   The AER allows each GPIP line to produce an interrupt on either a
 *   one-to-zero or a zero-to-one transition. Writing a zero to the
 *   appropriate edge bit of the AER causes the associated input to
 *   generate an interrupt on the one-to-zero transition. Writing a
 *   one to the edge bit produces an interrupt on the zero-to-one
 *   transition of the corresponding line. When the processor sets a
 *   bit, inter-rupts will be generated on the rising edge of the
 *   associated input signal. When the processor clears a bit,
 *   interrupts will be generated on the falling edge of the
 *   associated input signal.
 *
 *   [ GPIP7 | GPIP6 | GPIP5 | GPIP4 | GPIP3 | GPIP2 | GPIP1 | GPIPO ]
 *   RESET: [ 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 ]
 *
 *   @note The inputs to the exclusive-OR of the transition detector
 *         are the edge bit and the input buffer. As a result, writing
 *         the AER may cause an interrupt-producing transition,
 *         depending upon the state of the input. So, the AER should
 *         be configured before enabling interrupts via IERA and
 *         IERB. Also, changing the edge bit while interrupts are
 *         enabled may cause an interrupt on the corresponding
 *         channel.
 *
 * - 02 @b DDR (Data Direction Register):
 *
 *   Port's bit direction select. 0 => input / 1 => output.
 *
 *   The DDR allows the programmer to define I0-I7 as inputs or
 *   outputs by writing the corresponding bit. Writing a zero to any
 *   bit of the DDR causes the corresponding interrupt I/O pin to be a
 *   high-impedance input. Writing a one to any bit of the DDR causes
 *   the corresponding pin to be configured as a push-pull output.
 *
 *   [ GPIP7 | GPIP6 | GPIP5 | GPIP4 | GPIP3 | GPIP2 | GPIP1 | GPIPO ]
 *   RESET: [ 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 ]
 *
 *
 * - 03 @b IERA (Interrupt Enable Register A)
 * - 04 @b IERB (Interrupt Enable Register B)
 *
 *   Control the 16 interrupt source. 0 => Disable / 1 => Enable.
 *
 *   The interrupt channels are individually enabled or disabled by
 *   writing a one or a zero, respectively, to the appropriate bit of
 *   IERA or IERB. The processor may read these registers at any time.
 *
 *   When a channel is enabled, interrupts received on the channel are
 *   recognized by the MFP, and IRQ is asserted to the processor,
 *   indicating that interrupt service is required. On the other hand,
 *   a disabled channel is completely inactive; interrupts received on
 *   the channel are ignored by the MFP.
 *
 *   Writing a zero to a bit of IERA or IERB causes the corresponding
 *   bit of the IPR to be cleared, which terminates all interrupt
 *   service requests for the channel and also negates IRQ unless
 *   interrupts are pending from other sources. Dis-abling a channel,
 *   however, does not affect the corresponding bit in ISRA or
 *   ISRB. Therefore, if the MFP is in the software end-of-interrupt
 *   mode and an interrupt is in service when a channel is disabled,
 *   the in-service bit of that channel will remain set until cleared
 *   by software.
 *
 *   - iera bit#7 : bit 7 of I/O port (highest priority)
 *   - iera bit#6 : bit 6 of I/O port
 *   - iera bit#5 : Timer A
 *   - iera bit#4 : Receive Buffer Full
 *   - iera bit#3 : Receive Error
 *   - iera bit#2 : Send Buffer Empty
 *   - iera bit#1 : Send Error
 *   - iera bit#0 : Timer B
 *   - ierb bit#7 : bit 5 of I/O port
 *   - ierb bit#6 : bit 4 of I/O port
 *   - ierb bit#5 : Timer C
 *   - ierb bit#4 : Timer D
 *   - ierb bit#3 : bit 3 of I/O port
 *   - ierb bit#2 : bit 2 of I/O port
 *   - ierb bit#1 : bit 1 of I/O port
 *   - ierb bit#0 : bit 0 of I/O port (lowest priority)
 *
 * - 05 @b IPRA (Interrupt Pending Register A)
 * - 06 @b IPRB (Interrupt Pending Register B)
 *
 *   Bit is set if an interruption occurs. Interruption have to be
 *   enable (see IER). If the MFP can produce an interruption vector
 *   the bit is cleared automatically at this moment. In the contrary
 *   the bit should be clear by writing this register (~1<<bit).
 *
 *   When an interrupt is received on an enabled channel, the
 *   corresponding interrupt pending bit is set in IPRA or IPRB. In a
 *   vectored interrupt scheme, this bit will be cleared when the
 *   processor acknowledges the interrupting channel and the MFP
 *   responds with a vector number. In a polled interrupt scheme, the
 *   IPRs must be read to determine the interrupting channel, and then
 *   the interrupt pending bit is cleared by the interrupt handling
 *   routine without performing an interrupt acknowledge sequence.
 *
 * - 07 @b ISRA (Interrupt In Service Register A)
 * - 08 @b ISRB (Interrupt In Service Register B)
 *
 *   In AEI mode the bit is cleared after the MFP has produced the
 *   interruption vector and a new event can trigger a new
 *   interruption while another is been proced by the CPU. In SEI mode
 *   the bit is set and the interrupt routine have to clear it by
 *   writing this register (~1<<bit). While ISR bit is 1 all less
 *   prioritary bit are unmasked (?). When IPR is cleared (vector has
 *   been produced) a similar interrupt may occur. Anyway this
 *   interruption and the one less prioritary can be process only if
 *   ISR is disabled.
 *
 *   These registers indicate whether interrupt processing is in
 *   progress for a certain channel. A bit is set whenever an
 *   interrupt vector number is passed for a interrupt channel and the
 *   S-bit of the vector register is a one. The bit is cleared
 *   whenever interrupt service is complete for an associated
 *   interrupt channel, the S-bit of the vector register is cleared,
 *   or the processor writes a zero to the bit.
 *
 * - 09 @b IMRA (Interrupt Mask Register A)
 * - 0A @b IMRB (Interrupt Mask Register B)
 *
 *   If register allow ennable interrupt event to occur but not to
 *   produce an interrupt if corresponding bit in IMR is clear.
 *
 *   Interrupts are masked for a channel by clearing the appropriate
 *   bit in IMRA or IMRB. Even though an enabled channel is masked,
 *   the channel will recognize subsequent interrupts and set its
 *   interrupt pending bit. However, the channel is prevented from
 *   requesting interrupt service (!IRQ to the processor) as long as
 *   the mask bit for that channel is cleared. If a channel is
 *   requesting interrupt service at the time that its corresponding
 *   bit in IMRA or IMRB is cleared, the request will cease, and !IRQ
 *   will be negated unless another channel is requesting interrupt
 *   service. Later, when the mask bit is set, any pending interrupt
 *   on the channel will be processed according to the channel's
 *   assigned priority. IMRA and IMRB may be read at any time. Figure
 *   4 shows a conceptual circuit of an MFP interrupt channel.
 *
 * - 0B @b VR (Vector Register)
 *
 *   Vector Register (VR) This 8-bit register determines the four most
 *   significant bits in the interrupt vector format and which
 *   end-of-interrupt mode is used in a vectored interrupt scheme. The
 *   vector register should be written to before writing to the
 *   interrupt mask or enable registers to ensure that the MC68901
 *   responds to an interrupt acknowledge cycle with a vector number
 *   that is not in the range of allowable user vectors.
 *
 *   [ V7 | V6 | V5 | V4 | S | - | - | - ]
 *   Reset: [ 0 | 0 | 0 | 0 | 0 | U | U | U ]
 *
 *   - V7–V4 Written by user to set the most significant four bits of
 *     interrupt vector number.
 *   - S In-Service Register Enable
 *     - 1= Software end-of-interrupt mode and in-service register bits enabled.
 *     - 0= Automatic end-of-interrupt mode and in-service register bits forced low.
 *   - U Not used
 *
 * - 0C @b TACR  (Timer A Control Register) [0|0|0|R|E|A|A|A]
 * - 0D @b TBCR  (Timer B Control Register) [0|0|0|R|E|B|B|B]
 * - 0E @b TCDCR (Timer C/D Control Register) [0|C|C|C|0|D|D|D]
 * - 0F @b TADR  (Timer A Data Register) $00=$100
 * - 10 @b TBDR  (Timer B Data Register) $00=$100
 * - 11 @b TCDR  (Timer C Data Register) $00=$100
 * - 12 @b TDDR  (Timer D Data Register) $00=$100
 *
 *   @see Programming MFP Timers
 *
 * - 13 @b SCR (Synchronous Character Register)
 *
 *   Within a syncronous data transfer writing a specific value in
 *   this register use to start transfer as soon has it is recieved.
 *
 * - 14 UCR,USART (Control Register)
 *
 *   USART is Universal Synchronous/Asynchronous Receiver/Transmitter.
 *   - bit#0   : unused
 *   - bit#1   : 0:odd 1:even
 *   - bit#2   : 0:parity-off 1:parity-on
 *   - bit#3-4 : Transfert mode
 *              -00 Start 0, Stop 0, Synchronous
 *              -01 Start 1, Stop 1, Asynchronous
 *              -10 Start 1, Stop 1.5, Asynchronous
 *              -11 Start 1, Stop 2, Asynchronous
 *   - bit#5-6 : Data Length (8-value)
 *   - bit#7   : 0 use Timer-C frequency directly (synchornous only),
 *              1 use predivisor by 16
 *
 * - 15 RSR (Receiver Status Register)
 *
 *   - bit#0 : Receiver Enable Bit
 *   - bit#1 : Synchronous Strip Enable (sync)
 *   - bit#2 : Match (sync)/Character in progress (async)
 *   - bit#3 : Found-Search(sync)/Break Detect(async)
 *   - bit#4 : Frame Error
 *   - bit#5 : Parity Error
 *   - bit#6 : Overrun Error
 *   - bit#7 : Buffer Full
 *
 * - 16 TSR (Tranmitter Status Register)
 *
 *   - bit#0 : Transmitter Enable
 *   - bit#1 : High bit
 *   - bit#2 : Low bit
 *   - bit#3 : Break
 *   - bit#4 : End of transmission
 *   - bit#5 : Auto Turnaround
 *   - bit#6 : Underrun Error
 *   - bit#7 : Buffer Empty
 *
 * - 17 UDR,USART (DataRegister)
 *
 *      Data to send (write access) or recieve (read access)
 *
 * @p MFP interrupt vector table
 *
 *   MSB of the vector number (the x value below) is stored in the MFP
 *   vector register (register #11) bit#4 to bit#7.
 *
 * - x0 (initially disabled)-Parallel port interrupt handler
 * - x1 (initially disabled)-RS-232 carrier detect pin handler
 * - x2 (initially disabled)-RS-232 clear to send pin handler
 * - x3 (initially disabled)-Graphics blitter chip done interrupt
 *      handler (see below!)
 * - x4 (initially disabled) MFP Timer D done handler
 * - x5 200Hz System Clock (MFP Timer C) Handler
 * - x6 Keyboard or MIDI interrupt handler
 * - x7 (initially disabled) Floppy/hard disk data request handler
 * - x8 (initially disabled) Horizontal blank counter/MFP Timer B
 * - x9 RS-232 transmit error handler
 * - xA RS-232 transmit buffer empty handler
 * - xB RS-232 receive error handler
 * - xC RS-232 receive buffer full handler
 * - xD (initially disabled) MFP timer A
 * - xE (initially disabled) RS-232 ring detect pin
 * - xF (initially disabled) Monochrome/color monitor change detecter
 *
 *
 * Technical Summary
 *
 * Multifunction Peripheral
 *
 *   The MC68901 multifunction peripheral (MFP) is a member of the
 *   M68000 Family of peripherals. The MFP directly interfaces with
 *   the MC68000 microprocessor via the asynchronous bus
 *   structure. Both vectored and polled interrupt schemes are
 *   supported with the MFP, providing unique vector number generation
 *   for each of its 16 interrupt sources. Additionally, handshake
 *   lines are provided to facilitate direct memory access controller
 *   (DMAC) interfacing.
 *
 *   The MC68901 performs many of the functions common to most
 *   micro-processor-based systems. The following resources are
 *   available to the user:
 *
 *   * Eight Individually Programmable I/O Pins with Interrupt
 *     Capability
 *   * 16-Source Interrupt Controller with Individual Source Enable
 *     and Masking
 *   * Four Timers, Two of which Are Multimode Timers
 *   * Single-Channel, Full-Duplex Universal Synchronous/Asynchronous
 *     Receiver-Transmitter (USART) That Supports Asynchronous and,
 *     with the Addition of a Polynomial Generator Checker, Byte
 *     Synchronous Formats
 *
 * INTERRUPT PROCESSING
 *
 *   Each MFP provides individual interrupt capability for its various
 *   functions. When an interrupt is received on one of the external
 *   interrupt channels or from one of the internal interrupt
 *   channels, the MFP will request interrupt service. The 16
 *   interrupt channels are assigned a fixed priority so that multiple
 *   pending interrupts are serviced according to their relative
 *   importance. Since the MFP can generate 16 vector numbers, the
 *   unique vector number that corresponds to the highest priority
 *   channel having a pending interrupt is presented to the processor
 *   during an interrupt acknowledge cycle. This unique vector number
 *   allows the processor to immediately begin execution of the
 *   interrupt handler for the interrupting source, decreasing
 *   interrupt latency.
 *
 * Interrupt Channel Prioritization
 *
 *   The 16 interrupt channels are prioritized from highest (I7) to
 *   lowest (I0). The priority of the interrupt is determined by the
 *   least significant four bits in the interrupt vector number, which
 *   is internally generated by the MFP. Pending interrupts are
 *   presented to the processor in order of priority unless they have
 *   been masked. By selectively masking interrupts, the channels are,
 *   in effect, reprioritized.
 *
 * Interrupt Vector Number
 *
 *   During an interrupt acknowledge cycle, a unique 8-bit interrupt
 *   vector number, which corresponds to the specific interrupt source
 *   requesting service, is pre-sented to the system.
 *
 *   [ V7 | V6 | V5 |V4 | !V3 | !V2 | !V1 | !V0 ]
 *
 *   - V7-V4 — Copied from the vector register
 *   - !V3-!V0 — Determine highest priority channel requesting interrupt

 *
 * @p Nesting MFP Interrupts
 *
 *   In an M68000 vectored interrupt system, the MFP is assigned to
 *   one of seven possible interrupt levels. When an interrupt is
 *   received from the MFP, an interrupt acknowledge for that level is
 *   initiated. Once an interrupt is recognized at a particular level,
 *   interrupts at the same level or below are masked by the
 *   processor. As long as the processor's interrupt mask is
 *   unchanged, the M68000 interrupt structure prohibits nesting the
 *   interrupts at the same interrupt level. However, additional
 *   interrupt requests from the MFP can be recognized before a
 *   previous channel's interrupt service routine is finished by
 *   lowering the processor's interrupt mask to the next lower
 *   interrupt level within the interrupt handler.
 *
 *   When nesting MFP interrupts, it may be desirable to permit
 *   interrupts on any MFP channel, regardless of its priority, to
 *   preempt or delay interrupt processing of an earlier channel's
 *   interrupt service request. It may be desirable to only allow
 *   subsequent higher priority channel interrupt requests to
 *   supercede previously recognized lower priority interrupt
 *   requests. The MFP interrupt structure provides the flexibility by
 *   offering two end-of-interrupt options for vectored interrupt
 *   schemes. Note that the end-of-interrupt modes are not active in a
 *   polled interrupt scheme.
 *
 * @p GPIP general-Purpose IO Port
 *
 * GENERAL-PURPOSE I/O PORT
 *
 *   The general-purpose I/O port (GPIP) provides eight I/O lines
 *   (I0-I7) that may be operated as either inputs or outputs under
 *   software control. In addition, these lines may optionally
 *   generate an interrupt on either a positive transition or a
 *   negative transition of the input signal. The flexibility of the
 *   GPIP allows it to be configured as an 8-bit I/O port or for bit
 *   I/O. Since interrupts are enabled on a bit-by-bit basis, a subset
 *   of the GPIP can be programmed as handshake lines, or the port can
 *   be connected to as many as eight external interrupt sources,
 *   which would be prioritized by the MFP interrupt controller for
 *   the interrupt service.
 *
 * M6800 INTERRUPT CONTROLLER
 *
 *   The MFP interrupt controller is particularly useful in a system
 *   having many M6800-type devices.  Typically, in a vectored M68000
 *   system, M6800 peripherals use the autovector corresponding to
 *   their assigned interrupt level since they cannot provide a vector
 *   number in response to an interrupt acknowledge cycle. The
 *   autovector interrupt handler must then poll all M6800 devices at
 *   that interrupt level to determine which device is requesting
 *   service. However, by tying the IRQ output from an M6800
 *   peripheral to the GPIP of an MFP, a unique vector number is
 *   provided to the processor during an interrupt acknowledge
 *   cycle. This interrupt structure significantly reduces interrupt
 *   latency for M6800 devices and other peripherals that do not
 *   support vectored interrupts.
 *
 * GPIP CONTROL REGISTERS
 *
 *   The GPIP is programmed via three control registers. These
 *   registers control the data direction, provide user access to the
 *   port, and specify the active edge for each bit of the GPIP
 *   producing an interrupt. These registers are described in the
 *   following paragraphs.
 *
 * The GPIP registers and their usages are described above.
 *
 *   General-Purpose I/O Data Register (GPDR) The GPDR is used to
 *   input data from or output data to the port. When data is written
 *   to the GPDR, pins defined as inputs remain in the high-impedance
 *   state. Pins defined as outputs assume the state (high or low) of
 *   their corresponding bit in the data register. When the GPDR is
 *   read, data is passed directly from the bits of the data register
 *   for pins defined as outputs. Data from pins defined as inputs
 *   comes from the input buffers.
 *
 *   Active Edge Register (AER) The AER allows each GPIP line to
 *   produce an interrupt on either a one-to-zero or a zero-to-one
 *   transition. Writing a zero to the appropriate edge bit of the AER
 *   causes the associated input to generate an interrupt on the
 *   one-to-zero transition. Writing a one to the edge bit produces an
 *   interrupt on the zero-to-one transition of the corresponding
 *   line. When the processor sets a bit, interrupts will be generated
 *   on the rising edge of the associated input signal. When the
 *   processor clears a bit, interrupts will be generated on the
 *   falling edge of the associated input signal.
 *
 *   @note The inputs to the exclusive-OR of the transition detector
 *         are the edge bit and the input buffer. As a result, writing
 *         the AER may cause an interrupt-producing transition,
 *         depending upon the state of the input. So, the AER should
 *         be configured before enabling interrupts via IERA and
 *         IERB. Also, changing the edge bit while interrupts are
 *         enabled may cause an interrupt on the corresponding
 *         channel.  Data Direction Register (DDR) The DDR allows the
 *         programmer to define 10-17 as inputs or outputs by writing
 *         the corresponding bit. Writing a zero to any bit of the DDR
 *         causes the corresponding interrupt I/O pin to be a
 *         high-impedance input. Writing a one to any bit of the DDR
 *         causes the corresponding pin to be configured as a
 *         push-pull output.
 *
 * TIMERS
 *
 *   The MFP contains four 8-bit timers that provide many functions
 *   typically required in microprocessor systems. The timers can
 *   supply the baud rate clocks for the on-chip serial I/O channel,
 *   generate periodic interrupts, measure elapsed time, and count
 *   signal transitions. In addition, two timers have waveform
 *   generation capability.
 *
 *   All timers are prescalericounter timers with a common independent
 *   clock input (XTAL1 and XTAL2) and are not required to be operated
 *   from the system clock. Each timer's output signal toggles when
 *   the timer's main counter times out. Additionally, timers A and B
 *   have auxiliary control signals used in two of the operating
 *   modes. An interrupt channel is assigned to each timer; when the
 *   auxiliary control signals are used in the pulse-width measurement
 *   mode, a separate interrupt channel responds to transitions on
 *   these inputs.
 *
 * OPERATING MODES
 *
 *   Timers A and B are full-function timers which, in addition to the
 *   delay mode, operate in the pulse-width measurements mode and the
 *   event count mode. Timers C and D are delay timers only. A brief
 *   discussion of each of the timer modes follows.
 *
 * Delay Mode
 *
 *   All timers can operate in the delay mode. In this mode, the
 *   prescaler is always active. The prescaler specifies the number of
 *   timer clock cycles that must elapse before a count pulse is
 *   applied to the main counter. A count pulse causes the main
 *   counter to decrement by one. When the timer has decremented down
 *   to $01, the next count pulse causes the main counter to be
 *   reloaded from the timer data register, and a timeout pulse is
 *   produced. This timeout pulse is coupled to the timer's interrupt
 *   channel and, if the channel is enabled, an interrupt occurs. The
 *   timeout pulse also causes the timer output pin to toggle. The
 *   output will remain in this new state until the next timeout pulse
 *   occurs.
 *
 * Pulse-Width Measurement Mode
 *
 *   In addition to the delay mode, timers A and B can be programmed
 *   to operate in the pulse-width measurement mode. In this mode, an
 *   auxiliary control input is required; timers A and B auxiliary
 *   input lines are TAI and TBI. Also, interrupt channels normally
 *   associated with 14 and 13 respond instead to transitions on TAI
 *   and TBI, respectively. General-purpose lines 13 and 14 can still
 *   be used for I/O, but cannot be used as interrupt-generating
 *   inputs. A conceptual circuit of the interrupt source selection is
 *   shown in Figure 5.
 *
 *   The pulse-width measurement mode functions similarly to the delay
 *   mode, with the auxiliary control signal acting as an enable to
 *   the timer. When the control signal is active, the prescaler and
 *   main counter are allowed to operate. When the control signal is
 *   negated, the timer is stopped. Thus, the width of the active
 *   pulse on TAI or TBI is measured by the number of timer counts
 *   occurring while the timer is allowed to operate.
 *
 *   The active state of the auxiliary input line is defined by the
 *   associated interrupt channel's edge bit in the AER. The AER GPIP4
 *   edge bit is associated with TAI, and GPIP3 is associated with
 *   TB!. When the edge bit is a one, the auxiliary input will be
 *   active high, enabling the timer while the input signal is at a
 *   high level. If the edge bit is zero, the auxiliary input will be
 *   active low, and the timer will operate while the input signal is
 *   at a low level.
 *
 *   The active state of the auxiliary input line is defined by the
 *   associated interrupt channel's edge bit in the AER. The AER GPIP4
 *   edge bit is associated with TAI, and GPIP3 is associated with
 *   TBI. When the edge bit is a one, the auxiliary input will be
 *   active high, enabling the timer while the input signal is at a
 *   high level. If the edge bit is zero, the auxiliary input will be
 *   active low, and the timer will operate while the input signal is
 *   at a low level.
 *
 *   The state of the active edge bit also specifies whether a
 *   zero-to-one transition or a one-to-zero transition of the
 *   auxiliary input pin will produce an interrupt when the interrupt
 *   channel is enabled. In normal operation, programming the active
 *   edge bit to a one produces an interrupt on the zero-to-one
 *   transition of the associated input signal. Alternately,
 *   programming the edge bit to a zero produces an interrupt on the
 *   one-to-zero transition of the input signal. However, in the
 *   pulse-width measurement mode, the interrupt generated by a
 *   transition on TAI or TBI occurs on the opposite transition from
 *   that normally defined by the edge bit.
 *
 * Event Count Mode
 *
 *   In addition to the delay mode and the pulse-width measurement
 *   mode, timers A and B can be programmed to operate in the event
 *   count mode. Like the pulse-width measurement mode, the event
 *   count mode requires an auxiliary input signal, TAI or
 *   TBI. General-purpose lines 13 and 14 can be used for I/O or as
 *   interrupt-producing inputs.
 *
 *   In the event count mode, the prescaler is disabled, allowing each
 *   active transition on TAI and TBI to produce a count pulse. The
 *   count pulse causes the main counter to decrement by one. When the
 *   timer counts through $01, a timeout pulse is generated that
 *   causes the output signal to toggle and optionally produces an
 *   interrupt via the associated timer interrupt channel. The timer's
 *   main counter is also reloaded from the timer data register. To
 *   count transitions reliably, the input signal can only transition
 *   once every four timer clock periods. For this reason, the input
 *   signal must have a maximum frequency of one-fourth that of the
 *   timer clock.
 *
 * TIMER REGISTERS
 *
 *   The four timers are programmed via three control registers and
 *   four data registers. The following paragraphs describe the
 *   different registers.
 *
 * Timer Data Registers (TDRs)
 *
 *   The four TDRs are designed as timer A (TADR), timer B (TBDR),
 *   timer C (TCDR), and timer D (TDDR). Each timer's main counter is
 *   an 8-bit binary downcounter. The TDRs contain the value of their
 *   respective main counter. This value was captured on the last
 *   low-to-high transition of the data strobe pin.
 *
 *   The main counter is initialized by writing to the TDR. If the
 *   timer is stopped, data is loaded simultaneously into both the TDR
 *   and main counter. If the TDR is written to while the timer is
 *   enabled, the value is not loaded into the timer until the timer
 *   counts through $01. If a write is performed while the timer is
 *   counting through $01, then an indeterminate value is loaded into
 *   the main counter.
 *
 * Timer Control Registers (TCRs)
 *
 *   Timer control register A (TACR) and timer control register B
 *   (TBCR) are associated with timers A and B, respectively. Timers C
 *   and D are programmed using one control register—the timer C and D
 *   control register (TCDCR). The bits in the TCR select the
 *   operation mode, prescaler value, and disable the timers. Both
 *   TCRs have bits that allow the programmer to reset output lines
 *   TAO and TBO.
 *
 * Reset TAO/TBO — Reset Timer A/Timer B Output Lines
 *
 *   TAO and TBO can be forced low at any time by writing a one to the
 *   reset location in TACR and TBCR. Output is held low during the
 *   write operation, and at the end of the bus cycle, the output is
 *   allowed to toggle in response to a timeout pulse. When resetting
 *   TAO and TBO, the other bits in the TCR must be written with their
 *   previous value to avoid altering the operating mode.
 *
 * AC3-ACO/BC3-BCO — Select Timer A/Timer B Operation Mode
 *
 *   When the timer is stopped, counting is inhibited. The contents of
 *   the timer's main counter are not affected although any residual
 *   count in the prescaler is lost.
 *
 * CC3-CCO/DC3-DCO — Select Timer C/Timer D Operation Mode
 *
 *   When the timer is stopped, counting is inhibited. The contents of
 *   the timer's main counter are not affected although any residual
 *   count in the prescaler is lost.
 *
 * USART
 *
 *   The single-channel full-duplex USART has a double-buffered
 *   receiver and trans-mitter. There are separate receive and
 *   transmit clocks and separate receive and transmit status and data
 *   bytes. The receive and transmit sections are also assigned
 *   separate interrupt channels. Each section has two interrupt
 *   channels: one for normal conditions and one for error
 *   conditions. All interrupt channels are edge-triggered. Generally,
 *   it is the output of a flag bit or bits that is coupled to the
 *   interrupt channel. Thus, if an interrupt-producing event occurs
 *   while the associated interrupt channel is disabled, no interrupt
 *   is produced, even if the channel is subsequently enabled. That
 *   particular event would have to occur again, generating another
 *   edge, before an interrupt would be generated. The interrupt
 *   channels can be disabled, and a DMA device can be used to
 *   transfer the data via the control signals, receiver ready (RR)
 *   and transmitter ready (TR).
 *
 * CHARACTER PROTOCOLS
 *
 *   The MFP USART supports asynchronous and, with the help of a
 *   polynomial generator checker, byte synchronous character
 *   formats. These formats are selected independently of the
 *   divide-by-1 and divide-by-16 clock modes. It is possible to clock
 *   data synchronously into the MC68901 but still use start and stop
 *   bits. After a start bit is detected, data is shifted in and a
 *   stop bit is checked to determine proper framing. In this mode,
 *   all normal asynchronous format features apply.
 *
 *   When the divide-by-1 clock mode is selected, synchronization must
 *   be accom-plished externally. The receiver will sample serial data
 *   on the rising edge of the receiver clock. In the divide-by-16
 *   clock mode, the data is sampled at mid-bit time to increase
 *   transient noise rejection. Also, when the divide-by-16 clock mode
 *   is selected, the USART resynchronization logic is enabled,
 *   increasing the channel clock skew tolerance.
 *
 * Asynchronous Format
 *
 *   Variable character length and start/stop bit configurations are
 *   available under software control for asynchronous operation. The
 *   user can choose a character length from five to eight bits and a
 *   stop-bit length of one, one and one-half, or two bits. The user
 *   can also select odd, even, or no parity.
 *
 *   In the asynchronous format, start-bit detection is always
 *   enabled. New data is not shifted into the receiver shift register
 *   until a zero bit is received. When the divide-by-18 clock mode is
 *   selected, the false-start-bit logic is also active. Any
 *   transition must be stable for three positive receiver clock edges
 *   to be considered valid. For a start bit to be good, a valid
 *   zero-to-one transition must not occur for eight positive receiver
 *   clock transitions after the initial one-to-zero transition. After
 *   a valid start bit has been detected, the data is checked
 *   continuously for valid transitions. When a valid transition is
 *   detected, an internal counter is forced to state zero, and no
 *   further transition checking is initiated until state four. At
 *   state eight, the previous state of the transition checking logic
 *   is clocked into the receiver. As a result of this
 *   resynchronization logic, it is possible to run with asynchronous
 *   clocks without start and stop bits if there are sufficient valid
 *   transitions in the data streams.
 *
 * Synchronous Format
 *
 *   When the synchronous character format is selected, the 8-bit
 *   synchronous character loaded into the synchronous character
 *   register (SCR) is compared to received serial data until a match
 *   is found. Once synchronization is established, incoming data is
 *   clocked into the receiver. The synchronous word is continuously
 *   transmitted during an underrun condition. All synchronous
 *   characters can be optionally stripped from the receiver buffer
 *   (i.e., taken out of the data stream and thrown away) by clearing
 *   the appropriate bit in the receiver status register (RSR).
 *
 *   The synchronous character should be written after the character
 *   length is selected, since unused bits in the (SCR) are zeroed
 *   out. When parity is enabled, synchronous word length is the
 *   character length plus one. The MFP will compute and append the
 *   parity bit for the synchronous character when a character length
 *   of eight is selected. However, if the character length is less
 *   than eight, the user must determine the synchronous word parity
 *   and write it into the SCR along with the synchronous
 *   character. The parity bit must be the most significant bit. The
 *   MFP will then transmit the extra bit in the synchronous word as a
 *   parity bit.
 *
 * USART Control Register (UCR)
 *
 *   This register selects the clock mode and the character format for
 *   the receive and transmit sections.
 *
 * [ CLK | I | CL1 | I | CLO | I | STI | I | STO | I | PE | I | E10 | I ]
 *
 *   CLK - Clock Mode
 *
 *   - 1= Data clocked into and out of receiver and transmitter at
 *     one-sixteenth the frequency of their respective clocks.
 *   - 0= Data clocked into and out of receiver and transmitter at the
 *     frequency of their respective clocks.
 *
 *   CL1,CLO — Character Length
 *
 *   These bits specify the length of the character exclusive of start
 *   bits and parity.
 *
 *   - [0,0] -> 8-bits
 *   - [0,1] -> 7 Bits
 *   - [1,0] -> 6 Bits
 *   - [1,1] -> 5 Bits
 *
 * STO,ST1 — Start/Stop Bit and Format Control
 *
 *   These bits select the number of start and stop bits and specify
 *   the character format.
 *
 *   - [0,0,0,0] Synchronous
 *   - [0,1,1,1] Asynchronous
 *   - [1,0,1,1-1/2] Asynchronous (used with divide-by-16 mode only).
 *   - [1,1,1,2] Asynchronous
 *
 * PE — Parity Enable
 *
 *   Parity is not automatically appended to the synchronous character
 *   for char-acter lengths of less than eight bits. Therefore, parity
 *   should be written into the SCR along with the synchronous
 *   character.
 *
 *   - 1= Parity checked by receiver and parity calculated and
 *        inserted during data transmission.
 *   - 0= No parity check and no parity bit computed for
 *        transmission.
 *
 * E/O — Even/Odd Parity
 *
 *   - 1= Even parity is selected.
 *   - 0= Odd parity is selected.
 *
 * Bit 0 — Not used; read as zero
 *
 * RECEIVER
 *
 *   As data is received on the serial input line (SI), it is clocked
 *   into an internal 8-bit shift register until the specified number
 *   of data bits have been assembled. The character is then
 *   transferred to the receiver buffer, assuming that the last word
 *   in the receiver buffer has been read. This transfer sets the
 *   buffer full bit in the RSR and produces a buffer full interrupt
 *   to the processor, assuming this interrupt has been enabled.
 *
 *   Reading the receiver buffer satisfies the buffer full condition
 *   and allows a new data word to be transferred to the receiver
 *   buffer when it is assembled. The receiver buffer is accessed by
 *   reading the USART data register (UDR). The UDR is simply an 8-bit
 *   data register used for transferring data between the MFP and the
 *   CPU.
 *
 *   Each time a word is transferred to the receiver buffer, its
 *   status information is latched into the RSR. The RSR is not
 *   updated again until the data word in the receiver buffer has been
 *   read. When a buffer full condition exists, the RSR should always
 *   be read before the UDR to maintain the correct correspondence
 *   between data and flags. Otherwise, it is possible that, after
 *   reading the UDR and prior to reading the RSR, a new word could be
 *   received and transferred to the receiver buffer. Its associated
 *   flags would be latched into the RSR, writing over the flags for
 *   the previous data word. Thus, when the RSR is read to access the
 *   status information for the first data word, the flags for the new
 *   word would be retrieved.
 *
 *   Receiver Interrupt Channels
 *
 *     The USART receiver section is assigned two interrupt
 *     channels. One channel indicates the buffer full condition; the
 *     other channel indicates an error condition. Error conditions
 *     include overrun error, parity error, frame error, synchronous
 *     found, and break. These interrupting conditions correspond to
 *     the OE, PE, FE, and F/S or B bits of the RSR. These flags will
 *     function whether the receiver interrupt channels are enabled or
 *     disabled.
 *
 *     While only one interrupt is generated per character received,
 *     two dedicated interrupt channels allow separate vector numbers
 *     to be assigned for normal and abnormal receiver
 *     conditions. When a received word has an error asso-ciated with
 *     it and the error interrupt channel is enabled, an interrupt
 *     will be generated on the error channel only. However, if the
 *     error channel is disabled, an interrupt for an error condition
 *     will be generated on the buffer full interrupt channel along
 *     with interrupts produced by the buffer full condition. The RSR
 *     must always be read to determine which error condition produced
 *     the interrupt.
 *
 *   Receiver Status Register (RSR)
 *
 *     The RSR contains the receiver buffer full flag, the synchronous
 *     strip enable, the various status information associated with
 *     the data word in the receiver buffer. The RSR is latched each
 *     time a data word is transferred to the receiver buffer. RSR
 *     flags cannot change again until the new data word has been
 *     read. However, the M/CIP bit is allowed to change.
 *
 *     [ BF | OE | PE | FE | OF/S | ORB | WCIP | SS | RE ]
 *     Reset: 00000000
 *
 *     - BF - Buffer Full
 *       - 1= Receiver word is transferred to the receiver buffer.
 *       - 0= Receiver buffer is read by accessing the UDR.
 *
 *     - OE - Overrun Error Overrun error occurs when a received word
 *       is to be transferred to the receiver buffer but the buffer is
 *       full. Neither the receiver buffer nor the RSR is overwritten.
 *       - 1= Receiver buffer full
 *       - 0= Read by RSR or MFP reset
 *
 *    - PE — Parity Error
 *      - 1= Parity error detected on character transfer to receiver
 *        buffer
 *      - 0= No parity error detected on character transfer to receiver
 *        buffer
 *
 *     - FE — Frame Error
 *      A frame error exists when a nonzero data character is not
 *      followed by a stop bit in the asynchronous character format.
 *      - 1= Frame error detected on character transfer to receiver
 *        buffer
 *      - 0= No frame error detected on character transfer to
 *        receiver buffer
 *
 *     - F/S or B — Found/Search or Break Detect
 *
 *       The F/S bit is used in the synchronous character format. When
 *       set to zero, the USART receiver is placed in the search
 *       mode. PS is cleared when the incoming character does not
 *       match the synchronous character.
 *
 *       - 1= Match found; character length counter enabled.
 *       - 0= Incoming data compared to SCR; character length counter
 *         disabled.
 *
 *      The B bit is used in the asynchronous character format. This
 *      flag indicates a break condition that continues until a
 *      nonzero data bit is received.
 *       - 1= Character transferred to the receiver buffer is a break
 *         condition.
 *       - 0= Nonzero data bit received and break condition acknowledged
 *         by reading the RSR at least once.
 *
 *    - MICIP — Match/Character in Progress
 *
 *      The M bit is used in the synchronous character format and
 *      indicates a syn-chronous character has been received.
 *      - 1= Character transferred to the receiver buffer matches the
 *           synchronous character.
 *      - 0= Character transferred to the receiver buffer does not match
 *           the syn-chronous character.
 *
 *      The CIP bit is used in the asynchronous character format and
 *      indicates that a character is being assembled.
 *      - 1= Start bit is detected.
 *      - 0= Final stop bit has been received.
 *
 *    - SS — Synchronous Strip Enable
 *
 *      - 1= Characters that match the synchronous character are not
 *        loaded into the receiver buffer, and no buffer full condition
 *        is produced.
 *      - 0= Characters that match the synchronous character are
 *        transferred to the receiver buffer, and a buffer full
 *        condition is produced.
 *
 *    - RE — Receiver Enable
 *
 *      This bit should not be set until the receiver clock is
 *      active. When the trans-mitter is disabled in auto-turnaround
 *      mode, this bit is set.
 *      - 1= Receiver is enabled.
 *      - 0= Receiver is disabled.
 *
 * Special Receiver Conditions
 *
 *   Certain receiver conditions relating to the overrun error flag
 *   and the break detect flag require further explanation. Consider
 *   the following examples:
 *
 *   -# A break is received while the receiver buffer is full, which
 *      does not produce an overrun condition. Only the B flag will
 *      be set after the re-ceiver buffer is read.
 *
 *   -# A new word is received, and the receiver buffer is full. A break
 *      is received before the receiver buffer is read.
 *
 *   Both the B and OE flags will be set when the buffer full
 *   condition is satisfied.
 *
 * TRANSMITTER
 *
 *   The transmitter buffer is loaded by writing to the UDR. The data
 *   character will be transferred to an internal 8-bit shift register
 *   when the last character in the shift register has been
 *   transmitted. This transfer will produce a buffer empty
 *   condition. If the transmitter completes the transmission of the
 *   character in the shift register before a new character is written
 *   to the transmitter buffer, an underrun error will occur. In the
 *   asynchronous character format, the transmitter will send a mark
 *   until the transmitter buffer is written. In the synchronous
 *   character format, the transmitter will continuously send the
 *   synchronous character until the transmitter buffer is written.
 *
 *   The transmitter buffer can be loaded prior to enabling the
 *   transmitter. After the transmitter is enabled, a delay occurs
 *   before the first bit is output. The serial output line (SO)
 *   should be programmed to be high, low, or high impedance (by
 *   setting the appropriate bits in the transmitter status register
 *   (TSR)) before the transmitter is enabled, forcing the output line
 *   to the desired state until the first bit of the first character
 *   is shifted out. The state of the H and L bits in the TSR
 *   determine the state of the first transmitted bit after the
 *   transmitter is enabled. If the high-impedance mode is selected
 *   prior to the transmitter being enabled, the first bit transmitted
 *   is indeterminate. Note that the SO line will always be driven
 *   high for one bit time prior to the character in the trans-mitter
 *   shift register being transmitted when the transmitter is first
 *   enabled.
 *
 *   When the transmitter is disabled, any character currently being
 *   transmitted will continue to completion. However, any character
 *   in the transmitter buffer will not be transmitted and will remain
 *   in the buffer. Thus, no buffer empty condition will occur. If the
 *   buffer is empty when the transmitter is disabled, the buffer
 *   empty condition will remain, but no underrun condition will be
 *   generated when the character in transmission is completed. If no
 *   character is being transmitted when the transmitter is disabled,
 *   the transmitter will stop at the next rising edge of the internal
 *   shift clock.
 *
 *   In the asynchronous character format, the transmitter can be
 *   programmed to send a break. The break will be transmitted once
 *   the word currently in the shift register has been sent. If the
 *   shift register is empty, the break command will be effective
 *   immediately. A transmit error interrupt will be generated at
 *   every normal character boundary to aid in timing the break
 *   transmission. The contents of the TSR are not affected,
 *   however. The break will continue until the break bit is
 *   cleared. The underrun error bit must be cleared in the TSR. Also,
 *   the IPR must be cleared of pending transmitter errors at the
 *   beginning of the break transmission, or no interrupts will be
 *   generated at the character boundary time. The break bit cannot be
 *   set until the transmitter has been enabled and has had sufficient
 *   time (one transmitter clock cycle) to perform internal reset and
 *   initialization functions.
 *
 *   Any character in the transmitter buffer at the start of a break
 *   will be transmitted when the break is terminated, assuming the
 *   transmitter is still enabled. If the transmitter buffer is empty
 *   at the start of a break, it may be written at any time during the
 *   break. If the buffer is still empty at the end of the break, an
 *   underrun condition will exist.
 *
 *   Disabling the transmitter during a break condition causes the
 *   transmitter to cease transmission of the break character at the
 *   end of the current character. No end-of-break stop bit will be
 *   transmitted. Even if the transmitter buffer is empty, neither a
 *   buffer empty condition nor an underrun condition will
 *   occur. Also, any word in the transmitter buffer will remain.
 *
 * Transmitter Interrupt Channels
 *
 *   The USART transmitter section is assigned two interrupt
 *   channels. The normal channel indicates a buffer empty condition,
 *   and the error channel indicates an underrun or end
 *   condition. These interrupting conditions correspond to the BE,
 *   UE, and END flags in the TSR. The flag bits will function
 *   regardless of whether their associated interrupt channel is
 *   enabled or disabled.
 *
 * Transmitter Status Register (TSR)
 *
 *   The TSR contains various transmitter error flags and transmitter
 *   control bits for selecting auto-turnaround and loopback mode.
 *
 *   [ BE | UE | AT | I | ENO | B | I | H | L | TE ]
 *   Reset: 00000000
 *
 *   - BE Buffer Empty
 *     - 1= Character in the transmitter buffer transferred to transmit
 *       shift register.
 *     - 0= Transmitter buffer reloaded by writing to the UDR.
 *
 *   - UE Underrun Error
 *
 *     One full transmitter clock cycle is required after UE is set
 *     before it can be cleared. This bit does not require clearing
 *     before writing to the UDR.
 *     - 1= Character in the TSR was transmitted before a new word was
 *          loaded into the transmitter buffer.
 *     - 0= Transmitter disabled or a read performed on TSR.
 *
 *   - AT Auto-Turnaround
 *
 *     When set, the receiver will be enabled automatically after the
 *     transmitter has been disabled and the last character has been
 *     transmitted.
 *
 *   - END End of Transmission
 *
 *     If the transmitter is disabled while a character is being
 *     transmitted, this bit is set after transmission has
 *     completed. If no character is being transmitted, then this bit
 *     is set immediately. Reenabling the transmitter clears this bit.
 *
 *   - B Break
 *
 *     This bit only functions in the asynchronous format. When B is
 *     set, BE cannot be set. A break consists of all zeros with no
 *     stop bit. This bit cannot be set until the transmitter is
 *     enabled and internal reset and initialization is com-plete.
 *     - 1= Break is transmitted and transmission stops.
 *     - 0= Break ceases and transmission resumes.
 *
 *   - H,L High and Low
 *
 *     These bits configure SO when the transmitter is
 *     disabled. Changing these bits after the transmitter is enabled
 *     will alter the output state until END is cleared.
 *
 *     - [0,0] High Impedance
 *     - [0,1] Low
 *     - [1,0] High
 *     - [1,1] Loopback Mode
 *
 *   Loopback mode internally connects the transmitter output to the
 *   receiver input and the transmitter clock to the receiver clock
 *   internally. The receiver clock (RC) and the serial input (SI) are
 *   not used. When the transmitter is disabled, SO is forced high.
 *     - 1= MPU writes a one.
 *     - 0= MPU writes a zero.
 *
 *   TE Transmitter Enable
 *
 *     The serial output will be driven according to H and L bits
 *     until transmission begins. A one bit is transmitted before
 *     character transmission begins in the TSR.
 *     - 1= Transmitter enabled.
 *     - 0= Transmitter disabled. UE bit cleared and END bit set.
 *
 * DMA OPERATION
 *
 *   USART error conditions are valid only for each character
 *   boundary. When the USART performs block data transfers by using
 *   the DMA handshake lines, RR and TR, errors must be saved and
 *   checked at the end of a block. Checking is accomplished by
 *   enabling the error channel for the receiver or transmitter and by
 *   masking interrupts for this channel. Once the transfer is
 *   complete, IPRA is read. Any pending receiver or transmitter error
 *   indicates an error in the data transfer.
 *
 *   !RR is asserted when the RSR buffer full bit is set unless a
 *   parity error or frame error is detected by the receiver. !TR is
 *   asserted when the RSR buffer-empty bit is set unless a break is
 *   being transmitted.
 *
 * @{
 */

/**
 * @name MFP-68901 timers
 * @{
 */

/**
 * MFP-68901 timer identifers.
 */
enum mfp_timer_e {
  TIMER_A=0,   /**< MFP timer 'A' */
  TIMER_B,     /**< MFP timer 'B' */
  TIMER_C,     /**< MFP timer 'C' */
  TIMER_D      /**< MFP timer 'D' */
};


/**
 * CPU clock of the Atari ST is 8010613Hz. sc68 rounds it to 8010612Hz
 * (a mutliple of 4 because the YM clock is 1/4 of the CPU clock)
 * However usually the Atari ST clock and the YM clock are rounded to
 * respectively 8Mhz/2Mhz. Here in order to fasten the caculation the
 * CPU clock is approxinate to 8006400Hz. As the MFP timer clocks runs
 * at 2457600hz we got a power of 2 in the ratio (256:834).
 *
 * @{
 */

/**
 * bogo cycles (common denominator between CPU/MFP-timers clocks).
 */
typedef cycle68_t bogoc68_t;

#define MFP_BOGO_MUL 256u               /**< BOGO cycles/cpu clock  */
#define MFP_BOGO_DIV 834u               /**< BOGO cycle/mfp clock */
/**
 * @}
 */

/**
 * Timer definition struct.
 */
typedef struct {
  addr68_t vector; /**< Interrupt vector.                         */
  u8 level;        /**< Interrupt level.                          */
  u8 bit;          /**< Bit mask for Interrupt stuffes registers. */
  u8 channel;      /**< Channel A=0 or B=2.                       */
  u8 letter;       /**< 'A','B','C' or 'D'.                       */
} mfp_timer_def_t;

/**
 * MFP-68901 timer struct.
 */
typedef struct
{
  mfp_timer_def_t def; /**< Timer definition.                          */

  bogoc68_t cti;       /**< bogo-cycle to next timer interrupt.        */
  uint_t    tdr_cur;   /**< Timer Data register current value.         */
  uint_t    tdr_res;   /**< Timer Data register reset value.           */
  uint_t    tcr;       /**< Timer control register (prescaler) [0..7]. */

  bogoc68_t psc;       /**< prescale counter.                          */

  /* On Interrupt */
  uint_t   int_lost;   /**< Interrupts missed (should be 0).           */
  uint_t   int_mask;   /**< Interrupts ignored (masked or whatever)    */
  uint_t   int_fall;   /**< Interrupts counter.                        */

  interrupt68_t interrupt; /**< Interruption info.                     */
} mfp_timer_t;

/**
 * @}
 */

/**
 * MFP register name.
 */
enum {
  GPIP  = 0x01,
  AER   = 0x03,
  DDR   = 0x05,
  IERA  = 0x07,
  IERB  = 0x09,
  IPRA  = 0x0B,
  IPRB  = 0x0D,
  ISRA  = 0x0F,
  ISRB  = 0x11,
  IMRA  = 0x13,
  IMRB  = 0x15,
  VR    = 0x17,
  TACR  = 0x19,
  TBCR  = 0x1B,
  TCDCR = 0x1D,
  TADR  = 0x1F,
  TBDR  = 0x21,
  TCDR  = 0x23,
  TDDR  = 0x25,
  SCR   = 0x27,
  UCR   = 0x29,
  RSR   = 0x2B,
  TSR   = 0x2D,
  UDR   = 0x2F,
};

/**
 * MFP-68901 emulator. */
typedef struct {
  u8 map[0x40];                       /**< Registers map.        */
  mfp_timer_t timers[4];              /**< Timers.               */
} mfp_t;

/**
 * @name  MFP-68901 emulator library.
 * @{
 */

IO68_EXTERN
/**
 * MFP-68901 emulator library initialization.
 *
 *     The mfp_init() function intialize MFP emulator library.  It
 *     must be call prior to any other mfp_ function.
 *
 * @return  error-code
 * @retval  0  Success
 * @retval -1  Failure
 */
int mfp_init(void);

IO68_EXTERN
/**
 * MFP-68901 emulator library shutdown.
 */
void mfp_shutdown(void);

/**
 * @} */

/**
 * @name MFP-68901 emulator functions.
 * @{
 */

IO68_EXTERN
/**
 * Setup mfp instance.
 *
 * @param  mfp    mfp emulator instance.
 *
 * @return Error-code
 * @retval  0  Success
 * @retval -1  Failure
 */
int mfp_setup(mfp_t * const mfp);

IO68_EXTERN
/**
 * Cleanup mfp instance.
 *
 * @param  mfp    mfp emulator instance.
 *
 */
void mfp_cleanup(mfp_t * const mfp);


IO68_EXTERN
/**
 * Reset mfp instance.
 *
 * @param  mfp    mfp emulator instance.
 * @param  bogoc  bogo cycles
 *
 * @return Error-code
 * @retval  0  Success
 * @retval -1  Failure
 */
int mfp_reset(mfp_t * const mfp, const bogoc68_t bogoc);

IO68_EXTERN
/**
 * Destroy mfp instance.
 *
 * @param  mfp    mfp emulator instance.
 */
void mfp_destroy(mfp_t * const mfp);

/**
 * @}
 */

IO68_EXTERN
/**
 * MFP get Timer Data register.
 *
 * @param  mfp    mfp emulator instance.
 * @param  timer  Timer-id (see mfp_timer_e).
 * @param  bogoc  Current bogo-cycle.
 *
 * @return timer data register (TDR) value
 */
int68_t mfp_get_tdr(mfp_t * const mfp,
                    const int timer, const bogoc68_t bogoc);

IO68_EXTERN
/**
 * MFP write Timer data register.
 *
 * @param  mfp    mfp emulator instance.
 * @param  timer  Timer-id (see mfp_timer_e).
 * @param  v      New timer data register (TDR) value.
 * @param  bogoc  Current bogo-cycle.
 *
 */
void mfp_put_tdr(mfp_t * const mfp,
                 int timer, int68_t v, const bogoc68_t bogoc);

IO68_EXTERN
/**
 * MFP write Timer control register.
 *
 * @param  mfp    mfp emulator instance.
 * @param  timer  Timer-id (see mfp_timer_e).
 * @param  v      New timer control register (TCR) value.
 * @param  bogoc  Current bogo-cycle.
 *
 */
void mfp_put_tcr(mfp_t * const mfp,
                 int timer, int68_t v, const bogoc68_t bogoc);

IO68_EXTERN
/**
 * Get MFP pending interruption.
 *
 * @param  mfp    mfp emulator instance.
 * @param  bogoc  Current bogo-cycle.
 *
 * @return interruption info structure.
 * @retval 0 no pending interruption.
 *
 */
interrupt68_t * mfp_interrupt(mfp_t * const mfp, const bogoc68_t bogoc);

IO68_EXTERN
/**
 * Get cycle for the next MFP interruption.
 *
 * @param  mfp  mfp emulator instance.
 *
 * @return CPU-cycle when MFP will interrupt
 * @retval IO68_NO_INT no interrupt will occur.
 */
bogoc68_t mfp_nextinterrupt(const mfp_t * const mfp);

IO68_EXTERN
/**
 * Change cycle count base.
 *
 * @param  mfp    mfp emulator instance.
 * @param  bogoc  New base for internal cycle counter.
 */
void mfp_adjust_bogoc(mfp_t * const mfp, const bogoc68_t bogoc);

/**
 * @}
 */

#endif
