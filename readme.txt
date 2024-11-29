Il giocatore è rappresentato da una forma specificata nel config file
Spawn al centro della finestra, e ogni volta che muore torna lì
Il giocatore si muove con WASD ad una velocità scritta nel config file
Il giocatore non si muove oltre i limiti della finestra
Il giocatore spara un proiettile in direzione del mouse
    Velocità, grandezza e durata sono specificati nel config file

Abilità speciale, attivata col tasto destro
    Deve coinvolgere più entità sparate
    Grafica unica
    Nuova meccanica
    Timer di cooldown necessario

I nemici spawnano random dopo x frames, in config file
I nemici sono dentro la finestra
I nemici hanno x vertici, specificati nel config file
Il raggio è specificato nel file, così come il colore e la velocità (min e max)
I nemici rimbalzano alla stessa velocità sullo schermo
Quando i nemici muoiono spawnano N versioni piccole di loro con N uguale al numero di vertici e colore uguale
    Vanno all'infuori con angolo 360/N, ruotando su loro stessi ma non rispetto a dove si trovava prima il nemico

Lo score associato ad ogni figura è 100*N (N è il numero di vertici)
    Lo score è doppio per i nemici piccoli
Lo score dovrebbe essere visualizzato in alto a sx con il font nel config file

Il sistema di render fornisce rotazioni alle entità
Il parametro alpha del colore delle entità con un lifespan definito deve variare da 255 a 0, svanire.
    Bisogna usare dei timer.

Se premo P fermo il giocatore
Se premo Esc chiudo

Config File:

Window width, height, frameLimit, isFullscreen(0,1)
Font path, size, r, g, b
Player radius, collisionRadius, speed, fillColor r, g, b, outlineColor r, g, b, outlineThickness, numVertici
Enemy radius, collisionRadius, speedMin, speedMax, outlineColor r, g, b, outlineThickness, minVer, maxVert, lifespan, spawnInterval
    lifespan è per i nemici piccoli che vengono spawnati, il colore è casuale
Bullet radius, collisionRadius, speed, fillColor r, g, b, outlineColor r, g, b, outlineThickness, numVertici, lifespan



HINTS:
Implementa la classe Vec2




