# 12011EAU002-ATV2

Com o auxilio dos roteiros disponibilizados pelo professor, foi possível primeiramente desenvolver um código para a implementação do firmware que inicia o controlador e pisca o led presente na placa. Após isso, nessa atividade, utilizando o pino auxiliar PA0, que está ligado a um botão da placa STM32F401, foi implementado um código capaz de mudar a velocidade com que o led pisca.

Assim sendo, foi utilizado o endereço do GPIO deste botão e utilizado juntamemente com outras funções da biblioteca do STM para desenvolver a aplicação. O botão ficou configurado na forma input pull up.
