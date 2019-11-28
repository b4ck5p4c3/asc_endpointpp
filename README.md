# asc_endpointpp
Прошивка универсальной управляющей платы

# Карта регистров

* `0..15` режим (coil, 0 — вход, 1 — выход)
* `16..31` запись выхода (coil, 0 — logic low, 1 — logic high)
* `32..47` запись подтяжки (coil, 0 — pull down, 1 — pull up)
* `48..63` чтение входа с защелкой наверх (discrete, logic output)
* `64..79` чтение входа с защелкой вниз (discrete, logic outpu)
* `80..82` управление реле (coil, 0 — off, 1 — on)
