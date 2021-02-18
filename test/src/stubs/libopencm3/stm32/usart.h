/**
 * @file usart.h
 * @brief stubs for libopencm3 usart driver
 */

#ifndef USART_H_
#define USART_H_

#include <stdint.h>
#include <stdbool.h>

#define USART1			1
#define USART2			2
#define USART3			3
#define UART4				4
#define UART5				5

/* --- USART registers ----------------------------------------------------- */

/* --- USART_CR1 values ---------------------------------------------------- */

/* UE: USART enable */
#define USART_CR1_UE			(1 << 13)

/* M: Word length */
#define USART_CR1_M			(1 << 12)

/* WAKE: Wakeup method */
#define USART_CR1_WAKE			(1 << 11)

/* PCE: Parity control enable */
#define USART_CR1_PCE			(1 << 10)

/* PS: Parity selection */
#define USART_CR1_PS			(1 << 9)

/* PEIE: PE interrupt enable */
#define USART_CR1_PEIE			(1 << 8)

/* TXEIE: TXE interrupt enable */
#define USART_CR1_TXEIE			(1 << 7)

/* TCIE: Transmission complete interrupt enable */
#define USART_CR1_TCIE			(1 << 6)

/* RXNEIE: RXNE interrupt enable */
#define USART_CR1_RXNEIE		(1 << 5)

/* IDLEIE: IDLE interrupt enable */
#define USART_CR1_IDLEIE		(1 << 4)

/* TE: Transmitter enable */
#define USART_CR1_TE			(1 << 3)

/* RE: Receiver enable */
#define USART_CR1_RE			(1 << 2)

/* RWU: Receiver wakeup */
#define USART_CR1_RWU			(1 << 1)

/* SBK: Send break */
#define USART_CR1_SBK			(1 << 0)

/* --- USART_CR2 values ---------------------------------------------------- */

/* LINEN: LIN mode enable */
#define USART_CR2_LINEN			(1 << 14)

/* CLKEN: Clock enable */
#define USART_CR2_CLKEN			(1 << 11)

/* CPOL: Clock polarity */
#define USART_CR2_CPOL			(1 << 10)

/* CPHA: Clock phase */
#define USART_CR2_CPHA			(1 << 9)

/* LBCL: Last bit clock pulse */
#define USART_CR2_LBCL			(1 << 8)

/* LBDIE: LIN break detection interrupt enable */
#define USART_CR2_LBDIE			(1 << 6)

/* LBDL: LIN break detection length */
#define USART_CR2_LBDL			(1 << 5)

/* ADD[3:0]: Address of the usart node */
#define USART_CR2_ADD_MASK              0xF

/* --- USART_CR3 values ---------------------------------------------------- */

/* CTSIE: CTS interrupt enable */
/* Note: N/A on UART4 & UART5 */
#define USART_CR3_CTSIE			(1 << 10)

/* CTSE: CTS enable */
/* Note: N/A on UART4 & UART5 */
#define USART_CR3_CTSE			(1 << 9)

/* RTSE: RTS enable */
/* Note: N/A on UART4 & UART5 */
#define USART_CR3_RTSE			(1 << 8)

/* DMAT: DMA enable transmitter */
/* Note: N/A on UART5 */
#define USART_CR3_DMAT			(1 << 7)

/* DMAR: DMA enable receiver */
/* Note: N/A on UART5 */
#define USART_CR3_DMAR			(1 << 6)

/* SCEN: Smartcard mode enable */
/* Note: N/A on UART4 & UART5 */
#define USART_CR3_SCEN			(1 << 5)

/* NACK: Smartcard NACK enable */
/* Note: N/A on UART4 & UART5 */
#define USART_CR3_NACK			(1 << 4)

/* HDSEL: Half-duplex selection */
#define USART_CR3_HDSEL			(1 << 3)

/* IRLP: IrDA low-power */
#define USART_CR3_IRLP			(1 << 2)

/* IREN: IrDA mode enable */
#define USART_CR3_IREN			(1 << 1)

/* EIE: Error interrupt enable */
#define USART_CR3_EIE			(1 << 0)

#define USART_SR_CTS			(1 << 9)

/** LBD: LIN break detection flag */
#define USART_SR_LBD			(1 << 8)

/** TXE: Transmit data buffer empty */
#define USART_SR_TXE			(1 << 7)

/** TC: Transmission complete */
#define USART_SR_TC			(1 << 6)

/** RXNE: Read data register not empty */
#define USART_SR_RXNE			(1 << 5)

/** IDLE: Idle line detected */
#define USART_SR_IDLE			(1 << 4)

/** ORE: Overrun error */
#define USART_SR_ORE			(1 << 3)

/** NE: Noise error flag */
#define USART_SR_NE			(1 << 2)

/** FE: Framing error */
#define USART_SR_FE			(1 << 1)

/** PE: Parity error */
#define USART_SR_PE			(1 << 0)


/* Status register (USARTx_SR) */
#define USART_SR(usart_base)	usart_base
#define USART1_SR			0xFFFF
#define USART2_SR			0xFFFF
#define USART3_SR			0xFFFF
#define UART4_SR			0xFFFF
#define UART5_SR			0xFFFF

/* Data register (USARTx_DR) */
#define USART_DR(usart_base) (void)0
#define USART1_DR			USART1
#define USART2_DR			USART2
#define USART3_DR			USART3
#define UART4_DR			UART4
#define UART5_DR			UART5

/* Baud rate register (USARTx_BRR) */
#define USART_BRR(usart_base) (void)0
#define USART1_BRR		USART1
#define USART2_BRR		USART2
#define USART3_BRR		USART3
#define UART4_BRR			UART4
#define UART5_BRR			UART5

/* Control register 1 (USARTx_CR1) */
#define USART_CR1(usart_base) (void)0
#define USART1_CR1		0xFFFF
#define USART2_CR1		0xFFFF
#define USART3_CR1		0xFFFF
#define UART4_CR1			0xFFFF
#define UART5_CR1			0xFFFF

/* Control register 2 (USARTx_CR2) */
#define USART_CR2(usart_base) (void)0
#define USART1_CR2		USART1
#define USART2_CR2		USART2
#define USART3_CR2		USART3
#define UART4_CR2			UART4
#define UART5_CR2			UART5

/* Control register 3 (USARTx_CR3) */
#define USART_CR3(usart_base) (void)0
#define USART1_CR3		USART1
#define USART2_CR3		USART2
#define USART3_CR3		USART3
#define UART4_CR3			UART4
#define UART5_CR3			UART5

/* Guard time and prescaler register (USARTx_GTPR) */
#define USART_GTPR(usart_base) (void)0
#define USART1_GTPR			USART1
#define USART2_GTPR			USART2
#define USART3_GTPR			USART3
#define UART4_GTPR			UART4
#define UART5_GTPR			UART5


#define USART_PARITY_NONE		0x00
#define USART_PARITY_EVEN		0x01
#define USART_PARITY_ODD		0x02
#define USART_PARITY_MASK		0xFF

#define USART_MODE_RX           0x00
#define USART_MODE_TX		        0x01
#define USART_MODE_TX_RX	    	0x02
#define USART_MODE_MASK		      0xFF

#define USART_STOPBITS_1		    0x01   /* 1 stop bit */
#define USART_STOPBITS_0_5		  0x01 /* .5 stop bit */
#define USART_STOPBITS_2		    0x01   /* 2 stop bits */
#define USART_STOPBITS_1_5		  0x01 /* 1.5 stop bit*/

#define USART_CR2_STOPBITS_1		(0x00 << 12)     /* 1 stop bit */
#define USART_CR2_STOPBITS_0_5		(0x01 << 12)     /* 0.5 stop bits */
#define USART_CR2_STOPBITS_2		(0x02 << 12)     /* 2 stop bits */
#define USART_CR2_STOPBITS_1_5		(0x03 << 12)     /* 1.5 stop bits */
#define USART_CR2_STOPBITS_MASK		(0x03 << 12)
#define USART_CR2_STOPBITS_SHIFT	12

#define USART_FLOWCONTROL_NONE	        0x00
#define USART_FLOWCONTROL_RTS	          0x01
#define USART_FLOWCONTROL_CTS	          0x01
#define USART_FLOWCONTROL_RTS_CTS	      0x01
#define USART_FLOWCONTROL_MASK	        0xFF


void usart_set_baudrate(uint32_t usart, uint32_t baud);
void usart_set_databits(uint32_t usart, uint32_t bits);
void usart_set_stopbits(uint32_t usart, uint32_t stopbits);
void usart_set_parity(uint32_t usart, uint32_t parity);
void usart_set_mode(uint32_t usart, uint32_t mode);
void usart_set_flow_control(uint32_t usart, uint32_t flowcontrol);
void usart_enable(uint32_t usart);
void usart_disable(uint32_t usart);
void usart_send(uint32_t usart, uint16_t data);
uint16_t __attribute__((weak)) usart_recv(uint32_t usart);
void usart_wait_send_ready(uint32_t usart);
void usart_wait_recv_ready(uint32_t usart);
void __attribute__((weak)) usart_send_blocking(uint32_t usart, uint16_t data);
uint16_t usart_recv_blocking(uint32_t usart);
void usart_enable_rx_dma(uint32_t usart);
void usart_disable_rx_dma(uint32_t usart);
void usart_enable_tx_dma(uint32_t usart);
void usart_disable_tx_dma(uint32_t usart);
void usart_enable_rx_interrupt(uint32_t usart);
void usart_disable_rx_interrupt(uint32_t usart);
void usart_enable_tx_interrupt(uint32_t usart);
void usart_disable_tx_interrupt(uint32_t usart);
void usart_enable_error_interrupt(uint32_t usart);
void usart_disable_error_interrupt(uint32_t usart);
bool usart_get_flag(uint32_t usart, uint32_t flag);


#endif // USART_H_

