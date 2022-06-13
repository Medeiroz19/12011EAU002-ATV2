#include <stdlib.h>
#include <stdint.h>

/* AHB1 Base Addresses ******************************************************/
#define STM32_RCC_BASE 0x40023800 /* 0x40023800-0x40023bff: Reset and Clock control RCC */

/* AHB2 Base Addresses ******************************************************/
#define STM32_GPIOA_BASE 0x40020000U /* 0x40020000U-0x400203FFU: GPIO Port A */
#define STM32_GPIOC_BASE 0x40020800U /* 0x40020800-0x40020BFF: GPIO Port C */

/* Register Offsets *********************************************************/
#define STM32_RCC_AHB1ENR_OFFSET 0x0030 // AHB1 Peripheral Clock enableregister
#define STM32_GPIO_MODER_OFFSET 0x0000  // GPIO port mode register
#define STM32_GPIO_OTYPER_OFFSET 0x0004 // GPIO port output type register
#define STM32_GPIO_PUPDR_OFFSET 0x000c  // GPIO port pull-up/pull-downregister
#define STM32_GPIO_IDR_OFFSET 0x0010    // GPIO port input data register
#define STM32_GPIO_ODR_OFFSET 0x0014    // GPIO port output data register
#define STM32_GPIO_BSRR_OFFSET 0x0018   // GPIO port bit set/reset register

/* Register Addresses *******************************************************/
#define STM32_RCC_AHB1ENR (STM32_RCC_BASE + STM32_RCC_AHB1ENR_OFFSET)

#define STM32_GPIOA_MODER   (STM32_GPIOA_BASE + STM32_GPIO_MODER_OFFSET)
#define STM32_GPIOA_PUPDR   (STM32_GPIOA_BASE + STM32_GPIO_PUPDR_OFFSET)
#define STM32_GPIOA_IDR     (STM32_GPIOA_BASE + STM32_GPIO_IDR_OFFSET)

#define STM32_GPIOC_MODER (STM32_GPIOC_BASE + STM32_GPIO_MODER_OFFSET)
#define STM32_GPIOC_OTYPER (STM32_GPIOC_BASE + STM32_GPIO_OTYPER_OFFSET)
#define STM32_GPIOC_PUPDR (STM32_GPIOC_BASE + STM32_GPIO_PUPDR_OFFSET)
#define STM32_GPIOC_ODR (STM32_GPIOC_BASE + STM32_GPIO_ODR_OFFSET)
#define STM32_GPIOC_BSRR (STM32_GPIOC_BASE + STM32_GPIO_BSRR_OFFSET)

/* AHB1 Peripheral Clock enable register */
#define RCC_AHB1ENR_GPIOAEN (1 << 0) /* Bit 0: IO port A clock enable */
#define RCC_AHB1ENR_GPIOCEN (1 << 2) /* Bit 2: IO port C clock enable */

/* GPIO port mode register */
#define GPIO_MODER_INPUT (0)  
#define GPIO_MODER_OUTPUT (1) 
#define GPIO_MODER_ALT (2)    
#define GPIO_MODER_ANALOG (3)

/* GPIO port registers configs */
#define GPIO_MODERA0_SHIFT (0) 
#define GPIO_MODERA0_MASK (3 << GPIO_MODERA0_SHIFT)
#define GPIO_MODERC13_SHIFT (26)
#define GPIO_MODERC13_MASK (3 << GPIO_MODERC13_SHIFT)

/* GPIO port output type register */
#define GPIO_OTYPER_PP (0) /* 0=Output push-pull */
#define GPIO_OTYPER_OD (1) /* 1=Output open-drain */
#define GPIO_OTC13_SHIFT (13)
#define GPIO_OTC13_MASK (1 << GPIO_OTC13_SHIFT)

/* GPIO port pull-up/pull-down register */
#define GPIO_PUPDR_NONE (0)     /* No pull-up, pull-down */
#define GPIO_PUPDR_PULLUP (1)   /* Pull-up */
#define GPIO_PUPDR_PULLDOWN (2) /* Pull-down */

#define GPIO_PUPDRA0_SHIFT (0)
#define GPIO_PUPDRA0_MASK (3 << GPIO_PUPDRA0_SHIFT)
#define GPIO_PUPDRC13_SHIFT (26)
#define GPIO_PUPDRC13_MASK (3 << GPIO_PUPDRC13_SHIFT) 

/* GPIO port bit set/reset register */
#define GPIO_BSRR_SET(n) (1 << (n))
#define GPIO_BSRR_RST(n) (1 << (n + 16))

/* GPIO por IDR register */
#define GPIO_IDR0_SHIFT (0)
#define GPIO_IDRA0_MASK (1 << GPIO_IDR0_SHIFT)

#define LED_DELAY 100000

static uint32_t led_status, bot_status;

int main(int argc, char *argv[])
{
    uint32_t led_delay_time = LED_DELAY;
    uint32_t reg;

    /* Ponteiros para os rregistradores */
    uint32_t *pRCC_AHB1ENR = (uint32_t *)STM32_RCC_AHB1ENR;
    uint32_t *pGPIOC_MODER = (uint32_t *)STM32_GPIOC_MODER;
    uint32_t *pGPIOC_OTYPER = (uint32_t *)STM32_GPIOC_OTYPER;
    uint32_t *pGPIOC_PUPDR = (uint32_t *)STM32_GPIOC_PUPDR;
    uint32_t *pGPIOC_BSRR = (uint32_t *)STM32_GPIOC_BSRR;

    uint32_t *pGPIOA_MODER = (uint32_t *)STM32_GPIOA_MODER;
    uint32_t *pGPIOA_PUPDR = (uint32_t *)STM32_GPIOA_PUPDR;
    uint32_t *pGPIOA_IDR = (uint32_t *) STM32_GPIOA_IDR;

    /*Habilida o clock PC13 e configura*/
    reg = *pRCC_AHB1ENR;
    reg |= RCC_AHB1ENR_GPIOCEN;
    *pRCC_AHB1ENR = reg;
    reg = *pGPIOC_MODER;
    reg &= ~(GPIO_MODERC13_MASK);
    reg |= (GPIO_MODER_OUTPUT << GPIO_MODERC13_SHIFT);
    *pGPIOC_MODER = reg;
    reg = *pGPIOC_OTYPER;
    reg &= ~(GPIO_OTC13_MASK);
    reg |= (GPIO_OTYPER_PP << GPIO_OTC13_SHIFT);
    *pGPIOC_OTYPER = reg;
    reg = *pGPIOC_PUPDR;
    reg &= ~(GPIO_PUPDRC13_MASK);
    reg |= (GPIO_PUPDR_NONE << GPIO_PUPDRC13_SHIFT);
    *pGPIOC_PUPDR = reg;

    /*Habilita o clock PA0 e configura*/
    reg = *pRCC_AHB1ENR;
    reg |= RCC_AHB1ENR_GPIOAEN;
    *pRCC_AHB1ENR = reg;
    reg = *pGPIOA_MODER;
    reg &= ~(GPIO_MODERA0_MASK);
    reg |= (GPIO_MODER_INPUT << GPIO_MODERA0_SHIFT);
    *pGPIOA_MODER = reg;
    reg = *pGPIOA_PUPDR;
    reg &= ~(GPIO_PUPDRA0_MASK);
    reg |= (GPIO_PUPDR_PULLUP << GPIO_PUPDRA0_SHIFT);
    *pGPIOA_PUPDR = reg;


    while (1) /*Esse laço tem a função de ler o botão com o bot_status e de acordo com seu estado definir o tempo do delay*/
    {         /*Após realizar a leitura do tempo de delay é realizada a aplicação do delay na porta com o uso do BSRR*/
        bot_status = (GPIO_IDRA0_MASK & *pGPIOA_IDR); 
        if(bot_status == 0) 
        {
            led_delay_time = LED_DELAY;
        }
        else 
        {
            led_delay_time = LED_DELAY/2;
        }

        *pGPIOC_BSRR = GPIO_BSRR_SET(13);
        led_status = 0;
        for (uint32_t i = 0; i < led_delay_time; i++)
            if(bot_status != (GPIO_IDRA0_MASK & *pGPIOA_IDR)) break;

        *pGPIOC_BSRR = GPIO_BSRR_RST(13);
        led_status = 1;
        for (uint32_t i = 0; i < led_delay_time; i++)
            if(bot_status != (GPIO_IDRA0_MASK & *pGPIOA_IDR)) break;
        
    }

    return EXIT_SUCCESS;
}