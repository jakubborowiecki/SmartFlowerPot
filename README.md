# SmartFlowerPot

Link to folder with all files:
https://drive.google.com/drive/folders/1bVQrobbb8ZopDfx7F6vvX3TpBuy_JWUM?usp=drive_link


Polish version:

Projekt Inteligentna Doniczka to zaawansowany system wbudowany, opracowany w oparciu o mikrokontroler STM32 Nucleo-G491 i niskopoziomowe programowanie w języku C. Stanowi on kompleksową platformę przeznaczoną do automatycznego monitorowania kluczowych parametrów środowiskowych oraz precyzyjnego zarządzania procesem nawadniania upraw.

Architektura systemu opiera się na ciągłym pobieraniu i przetwarzaniu danych z pakietu zintegrowanych czujników. Moduł zbiera pomiary temperatury, ciśnienia, wilgotności gleby oraz wysokości cieczy w zbiorniku, dostarczając kompletny obraz warunków dla algorytmów sterujących.

Interfejs użytkownika realizowany jest za pomocą wyświetlacza prezentującego dane środowiskowe. Przycisk zapewnia funkcjonalność przełączania widoków na ekranie oraz inicjuje manualne załączanie pompy.

Zastosowanie potencjometru sprzężonego z wejściem analogowym mikrokontrolera (ADC) umożliwia płynną regulację kąta pracy serwomotoru. Dzięki temu możliwe jest precyzyjne dostosowanie strefy podlewania.

English version:

The Smart Plant Pot project is an advanced embedded system developed using the STM32 microcontroller and low-level programming in the C language. It constitutes a comprehensive platform designed for the automatic monitoring of key environmental parameters and the precise management of the crop irrigation process.

The system's architecture relies on the continuous acquisition and processing of data from an integrated sensor package. The module collects measurements of temperature, pressure, soil moisture, and liquid level in the reservoir, providing a complete picture of conditions for the control algorithms.

The user interface is implemented using a display that presents the environmental data. A push-button provides the functionality to switch views on the screen and initiates the manual activation of the pump.

The use of a potentiometer coupled with the microcontroller's analog-to-digital converter (ADC) input enables the smooth regulation of the servomotor's operating angle. This allows for the precise adjustment of the watering zone.
