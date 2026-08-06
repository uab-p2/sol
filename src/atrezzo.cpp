#include "atrezzo.h"
#include <iostream>

FakeAccessCard::FakeAccessCard()
    : m_active(false) {
    std::cout << "Personaje: Una tarjeta de acceso se desliza junto a un lector." << std::endl;
}

FakeAccessCard::~FakeAccessCard() {
    std::cout << "Personaje: La tarjeta de acceso desaparece del panel." << std::endl;
}

void FakeAccessCard::activate() {
    std::cout << "La tarjeta de acceso activa sus credenciales." << std::endl;
    m_active = true;
}

bool FakeAccessCard::unlock() const {
    bool success = false;
    if (!m_active) {
        std::cout << "La tarjeta de acceso no abre el cierre restringido." << std::endl;
    }
    else {
        std::cout << "La tarjeta de acceso desbloquea la puerta con un pitido." << std::endl;
        success = true;
    }
    return success;
}

FakeAlien::FakeAlien() {
    std::cout << "Personaje: Una alienígena aparece en silencio junto a la escotilla." << std::endl;
}

FakeAlien::~FakeAlien() {
    std::cout << "Personaje: La alienígena desaparece entre luces tenues." << std::endl;
}

void FakeAlien::greet(const FakeAstronaut& astronaut) const {
    static_cast<void>(astronaut);
    std::cout << "La alienígena inclina la cabeza y ofrece un saludo curioso." << std::endl;
}

bool FakeAlien::trade(FakeMerchant& merchant) const {
    FakeCoin coin;
    bool success = merchant.sell(coin);
    if (!success) {
        std::cout << "La alienígena guarda sus objetos y pospone el intercambio." << std::endl;
    }
    else {
        std::cout << "La alienígena acepta el trato con una sonrisa extraña." << std::endl;
    }
    return success;
}

bool FakeAlien::pet(FakeAlienPet& pet) const {
    FakePlant plant;
    plant.water();

    bool success = pet.eat(plant);
    if (!success) {
        std::cout << "La alienígena retira la mano para no asustar a su criatura." << std::endl;
    }
    else {
        std::cout << "La alienígena acaricia a su criatura y esta se calma." << std::endl;
    }
    return success;
}

FakeAlienPet::FakeAlienPet()
    : m_hidden(false) {
    std::cout << "Personaje: Una criatura pequeña asoma tras un panel." << std::endl;
}

FakeAlienPet::~FakeAlienPet() {
    std::cout << "Personaje: La pequeña criatura se pierde entre sombras." << std::endl;
}

void FakeAlienPet::follow(const FakeAlien& owner) {
    static_cast<void>(owner);
    std::cout << "La criatura sigue a su dueña con pasos rápidos y torpes." << std::endl;
    m_hidden = false;
}

bool FakeAlienPet::eat(FakePlant& plant) {
    bool success = plant.react();
    if (!success) {
        std::cout << "La criatura olisquea la planta, pero no se anima a morder." << std::endl;
    }
    else {
        std::cout << "La criatura picotea la planta con evidente entusiasmo." << std::endl;
    }
    return success;
}

void FakeAlienPet::hide() {
    std::cout << "La criatura se esconde detrás de una caja cercana." << std::endl;
    m_hidden = true;
}

FakeAndroid::FakeAndroid() {
    std::cout << "Personaje: Un androide activa sus sistemas con un zumbido suave." << std::endl;
}

FakeAndroid::~FakeAndroid() {
    std::cout << "Personaje: El androide entra en modo reposo." << std::endl;
}

bool FakeAndroid::calculate() const {
    bool success = true;
    if (!success) {
        std::cout << "El androide no cierra el cálculo por falta de datos." << std::endl;
    }
    else {
        std::cout << "El androide resuelve el cálculo en una fracción de segundo." << std::endl;
    }
    return success;
}

void FakeAndroid::assist(const FakeScientist& scientist) const {
    std::cout << "El androide prepara instrumentos y estabiliza la lectura." << std::endl;
    scientist.analyze(FakeCrystal());
}

bool FakeAndroid::recharge(FakeEnergyCell& cell) const {
    bool success = cell.use();
    if (!success) {
        cell.recharge();
        success = cell.use();
    }

    if (!success) {
        std::cout << "El androide no consigue restaurar su nivel de energía." << std::endl;
    }
    else {
        std::cout << "El androide recupera potencia y vuelve a operar." << std::endl;
    }
    return success;
}

FakeArchivist::FakeArchivist() {
    std::cout << "Personaje: Una archivera despliega cajas de registros antiguos." << std::endl;
}

FakeArchivist::~FakeArchivist() {
    std::cout << "Personaje: La archivera apila los legajos y cierra el depósito." << std::endl;
}

bool FakeArchivist::record(FakeStrangeArtifact& artifact) const {
    bool success = artifact.is_awake();
    if (!success) {
        artifact.awaken();
        success = artifact.is_awake();
    }

    if (!success) {
        std::cout << "La archivera no logra obtener datos verificables del objeto." << std::endl;
    }
    else {
        std::cout << "La archivera registra el objeto con referencia completa." << std::endl;
    }
    return success;
}

bool FakeArchivist::search(const FakeDataPad& pad) const {
    bool success = pad.read_note();
    if (!success) {
        std::cout << "La archivera no encuentra ninguna entrada relevante." << std::endl;
    }
    else {
        std::cout << "La archivera localiza la referencia en segundos." << std::endl;
    }
    return success;
}

void FakeArchivist::organize() const {
    std::cout << "La archivera ordena los registros por fecha y procedencia." << std::endl;
}

FakeAstronaut::FakeAstronaut() {
    std::cout << "Personaje: Una astronauta aparece lista para explorar." << std::endl;
}

FakeAstronaut::~FakeAstronaut() {
    std::cout << "Personaje: La astronauta abandona la zona de exploración." << std::endl;
}

bool FakeAstronaut::explore(FakeSpaceship& ship) const {
    ship.set_course();
    ship.launch();

    bool success = ship.travel();
    if (!success) {
        std::cout << "La astronauta retrasa la salida hasta tener mejores condiciones." << std::endl;
    }
    else {
        std::cout << "La astronauta sigue la ruta con confianza." << std::endl;
    }
    return success;
}

bool FakeAstronaut::collect(FakeStrangeArtifact& artifact) const {
    artifact.awaken();

    bool success = artifact.is_awake();
    if (!success) {
        std::cout << "La astronauta no logra activar el hallazgo." << std::endl;
    }
    else {
        std::cout << "La astronauta recoge el hallazgo con sumo cuidado." << std::endl;
    }
    return success;
}

void FakeAstronaut::wave(const FakeAlien& alien) const {
    static_cast<void>(alien);
    std::cout << "La astronauta saluda con calma a la visitante alienígena." << std::endl;
}

FakeBackpack::FakeBackpack()
    : m_packed(false) {
    std::cout << "Personaje: Una mochila cae suave sobre un banco de carga." << std::endl;
}

FakeBackpack::~FakeBackpack() {
    std::cout << "Personaje: La mochila queda fuera de escena." << std::endl;
}

void FakeBackpack::pack() {
    std::cout << "La mochila guarda equipo para una travesía larga." << std::endl;
    m_packed = true;
}

void FakeBackpack::unpack() {
    std::cout << "La mochila vacía su contenido sobre una mesa." << std::endl;
    m_packed = false;
}

bool FakeBackpack::is_packed() const {
    return m_packed;
}

FakeBlaster::FakeBlaster()
    : m_charged(false) {
    std::cout << "Personaje: Un bláster descansa sobre una mesa metálica." << std::endl;
}

FakeBlaster::~FakeBlaster() {
    std::cout << "Personaje: El bláster vuelve a su funda." << std::endl;
}

void FakeBlaster::charge() {
    std::cout << "El bláster acumula energía en su cámara." << std::endl;
    m_charged = true;
}

bool FakeBlaster::fire() {
    bool success = false;
    if (!m_charged) {
        std::cout << "El bláster emite un clic seco, sin disparo." << std::endl;
    }
    else {
        std::cout << "El bláster dispara un pulso de luz." << std::endl;
        m_charged = false;
        success = true;
    }
    return success;
}

FakeBountyHunter::FakeBountyHunter() {
    std::cout << "Personaje: Una cazarrecompensas revisa objetivos en un brazalete táctico." << std::endl;
}

FakeBountyHunter::~FakeBountyHunter() {
    std::cout << "Personaje: La cazarrecompensas abandona la zona sin mirar atrás." << std::endl;
}

bool FakeBountyHunter::chase(FakeSmuggler& smuggler) const {
    smuggler.sneak();

    bool success = true;
    if (!success) {
        std::cout << "La cazarrecompensas pierde la pista entre los pasillos." << std::endl;
    }
    else {
        std::cout << "La cazarrecompensas mantiene presión y corta rutas de escape." << std::endl;
    }
    return success;
}

bool FakeBountyHunter::capture(FakeAlien& alien) const {
    FakeAlienPet decoy;
    bool success = alien.pet(decoy);
    if (!success) {
        std::cout << "La cazarrecompensas no encuentra una ventana segura para actuar." << std::endl;
    }
    else {
        std::cout << "La cazarrecompensas inmoviliza al objetivo con precisión." << std::endl;
    }
    return success;
}

void FakeBountyHunter::aim(FakeBlaster& blaster) const {
    blaster.charge();
    bool success = blaster.fire();
    if (!success) {
        std::cout << "La cazarrecompensas reajusta la mira antes del siguiente intento." << std::endl;
    }
    else {
        std::cout << "La cazarrecompensas fija un tiro limpio y controlado." << std::endl;
    }
}

FakeBureaucrat::FakeBureaucrat() {
    std::cout << "Personaje: Una burócrata ordena formularios por prioridad." << std::endl;
}

FakeBureaucrat::~FakeBureaucrat() {
    std::cout << "Personaje: La burócrata archiva sellos y cierra ventanilla." << std::endl;
}

bool FakeBureaucrat::approve(FakeDataPad& form) const {
    form.write_note();
    bool success = form.read_note();
    if (!success) {
        std::cout << "La burócrata no aprueba nada sin datos completos." << std::endl;
    }
    else {
        std::cout << "La burócrata aprueba el trámite y estampa su visto bueno." << std::endl;
    }
    return success;
}

bool FakeBureaucrat::reject(FakeDataPad& form) const {
    bool success = form.read_note();
    if (!success) {
        std::cout << "La burócrata rechaza por formulario vacío." << std::endl;
    }
    else {
        std::cout << "La burócrata rechaza por un detalle de procedimiento." << std::endl;
    }
    return success;
}

void FakeBureaucrat::stamp() const {
    std::cout << "La burócrata deja un sello seco y perfectamente centrado." << std::endl;
}

FakeCaptain::FakeCaptain() {
    std::cout << "Personaje: Una capitana entra con presencia serena." << std::endl;
}

FakeCaptain::~FakeCaptain() {
    std::cout << "Personaje: La capitana se retira tras supervisar la misión." << std::endl;
}

void FakeCaptain::giveOrder(FakePilot& pilot) const {
    std::cout << "La capitana marca una maniobra y exige precisión." << std::endl;
    pilot.follow(FakeStarMap());
}

bool FakeCaptain::board(FakeSpaceship& ship) const {
    ship.dock();

    bool success = true;
    if (!success) {
        std::cout << "La capitana espera una pasarela estable para embarcar." << std::endl;
    }
    else {
        std::cout << "La capitana sube a bordo y toma el mando." << std::endl;
    }
    return success;
}

bool FakeCaptain::inspect(const FakeCargoCrate& crate) const {
    bool success = crate.is_open();
    if (!success) {
        std::cout << "La capitana encuentra el contenedor cerrado y sellado." << std::endl;
    }
    else {
        std::cout << "La capitana revisa el contenedor y valida su contenido." << std::endl;
    }
    return success;
}

FakeCargoCrate::FakeCargoCrate()
    : m_open(false) {
    std::cout << "Personaje: Un contenedor de carga se posiciona en el muelle." << std::endl;
}

FakeCargoCrate::~FakeCargoCrate() {
    std::cout << "Personaje: El contenedor de carga se retira del muelle." << std::endl;
}

void FakeCargoCrate::open() {
    std::cout << "El contenedor de carga abre sus cierres." << std::endl;
    m_open = true;
}

void FakeCargoCrate::close() {
    std::cout << "El contenedor de carga vuelve a quedar sellado." << std::endl;
    m_open = false;
}

bool FakeCargoCrate::is_open() const {
    return m_open;
}

FakeCoin::FakeCoin()
    : m_present(true) {
    std::cout << "Personaje: Una moneda universal rueda por el suelo." << std::endl;
}

FakeCoin::~FakeCoin() {
    std::cout << "Personaje: Un tintineo metálico se pierde a lo lejos." << std::endl;
}

void FakeCoin::polish() const {
    std::cout << "La moneda refleja la luz con un destello limpio." << std::endl;
}

bool FakeCoin::spend() {
    bool success = false;
    if (!m_present) {
        std::cout << "¿Es una moneda o es una chapa?" << std::endl;
    }
    else {
        std::cout << "Una mirada a la moneda demuestra que es real." << std::endl;
        m_present = false;
        success = true;
    }
    return success;
}

FakeCommunicator::FakeCommunicator()
    : m_online(false) {
    std::cout << "Personaje: Un comunicador se enciende con una luz intermitente." << std::endl;
}

FakeCommunicator::~FakeCommunicator() {
    std::cout << "Personaje: El comunicador queda en silencio." << std::endl;
}

void FakeCommunicator::tune() {
    std::cout << "El comunicador sintoniza un canal estable." << std::endl;
    m_online = true;
}

bool FakeCommunicator::call() const {
    bool success = false;
    if (!m_online) {
        std::cout << "El comunicador no logra enlazar la llamada." << std::endl;
    }
    else {
        std::cout << "El comunicador establece un enlace nítido." << std::endl;
        success = true;
    }
    return success;
}

FakeCourier::FakeCourier()
    : m_carrying_package(false) {
    std::cout << "Personaje: Una mensajera consulta una ruta urgente en su visor." << std::endl;
}

FakeCourier::~FakeCourier() {
    std::cout << "Personaje: La mensajera entrega su terminal y termina turno." << std::endl;
}

bool FakeCourier::deliver(FakeCargoCrate& crate) {
    bool success = false;
    if (!m_carrying_package) {
        std::cout << "La mensajera no puede entregar nada sin recoger antes el envío." << std::endl;
    }
    else {
        crate.close();
        m_carrying_package = false;
        std::cout << "La mensajera completa la entrega justo a tiempo." << std::endl;
        success = true;
    }
    return success;
}

bool FakeCourier::receive(FakeCargoCrate& crate) {
    crate.open();
    bool success = crate.is_open();
    if (!success) {
        std::cout << "La mensajera no consigue verificar la recogida." << std::endl;
    }
    else {
        m_carrying_package = true;
        std::cout << "La mensajera confirma la recogida y firma el albarán." << std::endl;
    }
    return success;
}

void FakeCourier::hurry() const {
    std::cout << "La mensajera acelera el paso entre pasillos abarrotados." << std::endl;
}

FakeCrystal::FakeCrystal()
    : m_glowing(false) {
    std::cout << "Personaje: Un cristal opaco espera una fuente de energía." << std::endl;
}

FakeCrystal::~FakeCrystal() {
    std::cout << "Personaje: El cristal se retira con cuidado." << std::endl;
}

void FakeCrystal::charge() {
    std::cout << "El cristal enciende un brillo irregular." << std::endl;
    m_glowing = true;
}

void FakeCrystal::dim() {
    std::cout << "El cristal baja su luz hasta quedar tenue." << std::endl;
    m_glowing = false;
}

bool FakeCrystal::is_glowing() const {
    return m_glowing;
}

FakeDataPad::FakeDataPad()
    : m_has_note(false) {
    std::cout << "Personaje: Una tableta de datos muestra su pantalla de inicio." << std::endl;
}

FakeDataPad::~FakeDataPad() {
    std::cout << "Personaje: La tableta de datos se apaga." << std::endl;
}

void FakeDataPad::write_note() {
    std::cout << "La tableta de datos guarda una nota breve." << std::endl;
    m_has_note = true;
}

bool FakeDataPad::read_note() const {
    bool success = false;
    if (!m_has_note) {
        std::cout << "La tableta de datos no encuentra notas guardadas." << std::endl;
    }
    else {
        std::cout << "La tableta de datos recupera la nota al instante." << std::endl;
        success = true;
    }
    return success;
}

FakeDetective::FakeDetective()
    : m_balanced(true) {
    std::cout << "Personaje: Un detective llega tomando notas apresuradas." << std::endl;
}

FakeDetective::~FakeDetective() {
    std::cout << "Personaje: El detective se marcha con más preguntas que respuestas." << std::endl;
}

bool FakeDetective::investigate() const {
    bool success = m_balanced;
    if (!success) {
        std::cout << "El detective pierde el hilo tras un tropiezo inoportuno." << std::endl;
    }
    else {
        std::cout << "El detective conecta pistas con intuición sorprendente." << std::endl;
    }
    return success;
}

bool FakeDetective::inspect(const FakeStrangeArtifact& artifact) const {
    bool success = artifact.is_awake();
    if (!success) {
        std::cout << "El detective ve un objeto raro, pero ninguna pista firme." << std::endl;
    }
    else {
        std::cout << "El detective detecta marcas recientes alrededor del objeto." << std::endl;
    }
    return success;
}

void FakeDetective::trip() {
    std::cout << "El detective tropieza con un cable en el peor momento." << std::endl;
    m_balanced = false;
}

FakeDiplomat::FakeDiplomat() {
    std::cout << "Personaje: Una diplomática entra con carpeta y protocolo." << std::endl;
}

FakeDiplomat::~FakeDiplomat() {
    std::cout << "Personaje: La diplomática cierra la sesión y guarda actas." << std::endl;
}

bool FakeDiplomat::negotiate(FakeAlien& alien) const {
    static_cast<void>(alien);

    bool success = true;
    if (!success) {
        std::cout << "La diplomática pospone el acuerdo por falta de consenso." << std::endl;
    }
    else {
        std::cout << "La diplomática encauza la conversación hacia un acuerdo viable." << std::endl;
    }
    return success;
}

void FakeDiplomat::greet(const FakeCaptain& captain) const {
    static_cast<void>(captain);
    std::cout << "La diplomática ofrece un saludo formal antes de negociar." << std::endl;
}

bool FakeDiplomat::sign(FakeDataPad& pad) const {
    pad.write_note();
    bool success = pad.read_note();
    if (!success) {
        std::cout << "La diplomática no puede firmar sin documento visible." << std::endl;
    }
    else {
        std::cout << "La diplomática firma el acuerdo en la tableta." << std::endl;
    }
    return success;
}

FakeDog::FakeDog()
    : m_friendly(false) {
    std::cout << "Personaje: Un perro mira a su alrededor con desconfianza." << std::endl;
}

FakeDog::~FakeDog() {
    std::cout << "Personaje: Un perro se va a dormir a una esquina." << std::endl;
}

void FakeDog::feed() {
    std::cout << "El perro devora unas sobras." << std::endl;
    m_friendly = true;
}

bool FakeDog::pet() const {
    bool success = false;
    if (!m_friendly) {
        std::cout << "Al acercar la mano, el perro muerde." << std::endl;
    }
    else {
        std::cout << "Con una caricia, el perro se deja." << std::endl;
        success = true;
    }
    return success;
}

FakeEnergyCell::FakeEnergyCell()
    : m_charged(true) {
    std::cout << "Personaje: Una célula de energía late con carga estable." << std::endl;
}

FakeEnergyCell::~FakeEnergyCell() {
    std::cout << "Personaje: La célula de energía se desconecta." << std::endl;
}

void FakeEnergyCell::recharge() {
    std::cout << "La célula de energía recupera su carga máxima." << std::endl;
    m_charged = true;
}

bool FakeEnergyCell::use() {
    bool success = false;
    if (!m_charged) {
        std::cout << "La célula de energía no entrega más potencia." << std::endl;
    }
    else {
        std::cout << "La célula de energía alimenta el sistema conectado." << std::endl;
        m_charged = false;
        success = true;
    }
    return success;
}

FakeExplorer::FakeExplorer() {
    std::cout << "Personaje: Una exploradora marca nuevos puntos en su mapa." << std::endl;
}

FakeExplorer::~FakeExplorer() {
    std::cout << "Personaje: La exploradora regresa con polvo estelar en las botas." << std::endl;
}

bool FakeExplorer::discover(FakeStrangeArtifact& artifact) const {
    artifact.awaken();

    bool success = artifact.is_awake();
    if (!success) {
        std::cout << "La exploradora no encuentra señales claras de descubrimiento." << std::endl;
    }
    else {
        std::cout << "La exploradora descubre un objeto de origen incierto." << std::endl;
    }
    return success;
}

void FakeExplorer::follow(const FakeStarMap& map) const {
    bool success = map.show_route();
    if (!success) {
        std::cout << "La exploradora espera a que se defina un rumbo útil." << std::endl;
    }
    else {
        std::cout << "La exploradora avanza por una senda poco transitada." << std::endl;
    }
}

bool FakeExplorer::climb(FakePlant& plant) const {
    bool success = plant.react();
    if (!success) {
        std::cout << "La exploradora evita trepar para no romper la planta." << std::endl;
    }
    else {
        std::cout << "La exploradora usa la planta como apoyo natural." << std::endl;
    }
    return success;
}

FakeFortuneTeller::FakeFortuneTeller() {
    std::cout << "Personaje: Una vidente enciende luces tenues alrededor de su mesa." << std::endl;
}

FakeFortuneTeller::~FakeFortuneTeller() {
    std::cout << "Personaje: La vidente apaga su mesa y recoge sus cartas." << std::endl;
}

bool FakeFortuneTeller::predict(const FakeAstronaut& astronaut) const {
    static_cast<void>(astronaut);
    std::cout << "La vidente traza un destino brillante para la próxima travesía." << std::endl;

    bool success = true;
    return success;
}

void FakeFortuneTeller::read(FakeCrystal& crystal) const {
    crystal.charge();
    std::cout << "La vidente interpreta destellos cambiantes dentro del cristal." << std::endl;
}

void FakeFortuneTeller::smile() const {
    std::cout << "La vidente sonríe como si ya conociera el desenlace." << std::endl;
}

FakeGravityBoots::FakeGravityBoots()
    : m_enabled(false) {
    std::cout << "Personaje: Unas botas gravitatorias quedan alineadas junto a la compuerta." << std::endl;
}

FakeGravityBoots::~FakeGravityBoots() {
    std::cout << "Personaje: Las botas gravitatorias se guardan en su caja." << std::endl;
}

void FakeGravityBoots::enable() {
    std::cout << "Las botas gravitatorias anclan la pisada con firmeza." << std::endl;
    m_enabled = true;
}

void FakeGravityBoots::disable() {
    std::cout << "Las botas gravitatorias liberan su campo de sujeción." << std::endl;
    m_enabled = false;
}

bool FakeGravityBoots::walk_any_surface() const {
    bool success = false;
    if (!m_enabled) {
        std::cout << "Las botas gravitatorias no estabilizan sin energía." << std::endl;
    }
    else {
        std::cout << "Las botas gravitatorias mantienen tracción en cualquier plano." << std::endl;
        success = true;
    }
    return success;
}

FakeHelmet::FakeHelmet()
    : m_worn(false) {
    std::cout << "Personaje: Un casco reposa sobre un soporte iluminado." << std::endl;
}

FakeHelmet::~FakeHelmet() {
    std::cout << "Personaje: El casco se retira del encuadre." << std::endl;
}

void FakeHelmet::wear() {
    std::cout << "El casco encaja con un cierre firme." << std::endl;
    m_worn = true;
}

void FakeHelmet::remove() {
    std::cout << "El casco se separa y deja la visera abierta." << std::endl;
    m_worn = false;
}

bool FakeHelmet::is_worn() const {
    return m_worn;
}

FakeHologramProjector::FakeHologramProjector() {
    std::cout << "Personaje: Un proyector holográfico se coloca en el centro de la sala." << std::endl;
}

FakeHologramProjector::~FakeHologramProjector() {
    std::cout << "Personaje: El proyector holográfico se apaga." << std::endl;
}

void FakeHologramProjector::project_map() const {
    std::cout << "El proyector holográfico dibuja un mapa flotante." << std::endl;
}

void FakeHologramProjector::project_message() const {
    std::cout << "El proyector holográfico muestra un mensaje suspendido." << std::endl;
}

void FakeHologramProjector::clear() const {
    std::cout << "El proyector holográfico borra la imagen en el aire." << std::endl;
}

FakeInventor::FakeInventor() {
    std::cout << "Personaje: Una inventora entra cargando piezas sin etiquetar." << std::endl;
}

FakeInventor::~FakeInventor() {
    std::cout << "Personaje: La inventora recoge el banco de pruebas y desconecta todo." << std::endl;
}

bool FakeInventor::build(FakeRobotDrone& drone) const {
    drone.activate();
    bool success = drone.assist();
    if (!success) {
        std::cout << "La inventora no logra arrancar el prototipo." << std::endl;
    }
    else {
        std::cout << "La inventora completa un dron funcional contra pronóstico." << std::endl;
    }
    return success;
}

bool FakeInventor::upgrade(FakeCommunicator& device) const {
    device.tune();
    bool success = device.call();
    if (!success) {
        std::cout << "La inventora detecta interferencias y aplaza la mejora." << std::endl;
    }
    else {
        std::cout << "La inventora mejora el comunicador y estabiliza su alcance." << std::endl;
    }
    return success;
}

void FakeInventor::test() const {
    std::cout << "La inventora lanza una prueba rápida y toma nota del resultado." << std::endl;
}

FakeJanitor::FakeJanitor() {
    std::cout << "Personaje: Una conserje aparece con carro y útiles de limpieza." << std::endl;
}

FakeJanitor::~FakeJanitor() {
    std::cout << "Personaje: La conserje deja el pasillo impecable y se retira." << std::endl;
}

void FakeJanitor::clean(FakeSpaceship& ship) const {
    ship.dock();
    std::cout << "La conserje limpia la zona de embarque sin interrumpir maniobras." << std::endl;
}

bool FakeJanitor::find(FakeCoin& coin) const {
    bool success = coin.spend();
    if (!success) {
        std::cout << "La conserje cree encontrar una moneda." << std::endl;
    }
    else {
        std::cout << "La conserje encuentra una moneda y la recoge del suelo." << std::endl;
    }
    return success;
}

void FakeJanitor::whistle() const {
    std::cout << "La conserje silba una melodía mientras repasa el suelo." << std::endl;
}

FakeJetpack::FakeJetpack()
    : m_active(false) {
    std::cout << "Personaje: Un jetpack está abandonado en un banco." << std::endl;
}

FakeJetpack::~FakeJetpack() {
    std::cout << "Personaje: El jetpack se desmaterializa." << std::endl;
}

void FakeJetpack::ignite() {
    std::cout << "El jetpack da un petardazo y arranca." << std::endl;
    m_active = true;
}

void FakeJetpack::shutdown() {
    std::cout << "El jetpack se queda mudo de golpe." << std::endl;
    m_active = false;
}

bool FakeJetpack::boost() const {
    bool success = false;
    if (!m_active) {
        std::cout << "El jetpack no responde." << std::endl;
    }
    else {
        std::cout << "El jetpack responde con un bramido." << std::endl;
        success = true;
    }
    return success;
}

FakeMechanic::FakeMechanic() {
    std::cout << "Personaje: Una mecánica llega con las manos llenas de grasa." << std::endl;
}

FakeMechanic::~FakeMechanic() {
    std::cout << "Personaje: La mecánica guarda sus herramientas y se marcha." << std::endl;
}

bool FakeMechanic::repair(FakeSpaceship& ship) const {
    ship.dock();

    bool success = true;
    if (!success) {
        std::cout << "La mecánica no puede trabajar sin acceso al fuselaje." << std::endl;
    }
    else {
        std::cout << "La mecánica deja la nave lista para otra salida." << std::endl;
    }
    return success;
}

bool FakeMechanic::repair(FakeRobotDrone& drone) const {
    drone.activate();

    bool success = drone.assist();
    if (!success) {
        std::cout << "La mecánica no logra reactivar el dron." << std::endl;
    }
    else {
        std::cout << "La mecánica recalibra el dron y vuelve al servicio." << std::endl;
    }
    return success;
}

void FakeMechanic::use(const FakeToolbox& toolbox) const {
    bool success = toolbox.has_tools();
    if (!success) {
        std::cout << "La mecánica improvisa mientras busca una llave útil." << std::endl;
    }
    else {
        std::cout << "La mecánica elige la herramienta exacta al primer intento." << std::endl;
    }
}

FakeMerchant::FakeMerchant() {
    std::cout << "Personaje: Un mercader abre su puesto con gesto atento." << std::endl;
}

FakeMerchant::~FakeMerchant() {
    std::cout << "Personaje: El mercader cierra el puesto y recoge sus cajas." << std::endl;
}

bool FakeMerchant::sell(FakeCoin& coin) const {
    bool success = coin.spend();
    if (!success) {
        std::cout << "El mercader rechaza el pago y retira el producto." << std::endl;
    }
    else {
        std::cout << "El mercader entrega el producto tras confirmar el pago." << std::endl;
    }
    return success;
}

bool FakeMerchant::buy(FakeStrangeArtifact& artifact) const {
    artifact.awaken();
    bool success = artifact.is_awake();
    if (!success) {
        std::cout << "El mercader no ve valor inmediato en el objeto." << std::endl;
    }
    else {
        std::cout << "El mercader acepta la compra tras revisar el brillo del objeto." << std::endl;
    }
    return success;
}

void FakeMerchant::negotiate(FakeDiplomat& diplomat) const {
    static_cast<void>(diplomat);
    std::cout << "El mercader ajusta precios mientras mantiene el tono cordial." << std::endl;
}

FakeMiner::FakeMiner() {
    std::cout << "Personaje: Una minera ajusta su equipo antes de perforar." << std::endl;
}

FakeMiner::~FakeMiner() {
    std::cout << "Personaje: La minera limpia su traje y guarda el taladro." << std::endl;
}

bool FakeMiner::mine(FakeCrystal& crystal) const {
    crystal.charge();
    bool success = crystal.is_glowing();
    if (!success) {
        std::cout << "La minera no extrae material útil en este intento." << std::endl;
    }
    else {
        std::cout << "La minera extrae fragmentos brillantes de gran pureza." << std::endl;
    }
    return success;
}

bool FakeMiner::load(FakeCargoCrate& crate) const {
    crate.open();
    bool success = crate.is_open();
    if (!success) {
        std::cout << "La minera no puede cargar el contenedor cerrado." << std::endl;
    }
    else {
        std::cout << "La minera carga el contenedor y asegura el contenido." << std::endl;
        crate.close();
    }
    return success;
}

void FakeMiner::dig() const {
    std::cout << "La minera perfora con ritmo constante sobre roca densa." << std::endl;
}

FakePilot::FakePilot() {
    std::cout << "Personaje: Un piloto revisa paneles de navegación." << std::endl;
}

FakePilot::~FakePilot() {
    std::cout << "Personaje: El piloto deja la cabina en orden." << std::endl;
}

bool FakePilot::fly(FakeSpaceship& ship) const {
    ship.set_course();
    ship.launch();

    bool success = ship.travel();
    if (!success) {
        std::cout << "El piloto cancela el despegue por falta de condiciones." << std::endl;
    }
    else {
        std::cout << "El piloto sostiene un vuelo estable de largo alcance." << std::endl;
    }
    return success;
}

bool FakePilot::land(FakeSpaceship& ship) const {
    ship.dock();

    bool success = true;
    if (!success) {
        std::cout << "El piloto aborta el descenso en el último segundo." << std::endl;
    }
    else {
        std::cout << "El piloto completa un aterrizaje limpio." << std::endl;
    }
    return success;
}

void FakePilot::follow(const FakeStarMap& map) const {
    bool success = map.show_route();
    if (!success) {
        std::cout << "El piloto espera a que aparezca una ruta fiable." << std::endl;
    }
    else {
        std::cout << "El piloto corrige el rumbo con exactitud." << std::endl;
    }
}

FakePlant::FakePlant()
    : m_watered(false) {
    std::cout << "Personaje: Una planta alienígena asoma hojas nuevas en su maceta." << std::endl;
}

FakePlant::~FakePlant() {
    std::cout << "Personaje: La planta alienígena se repliega en su maceta." << std::endl;
}

void FakePlant::water() {
    std::cout << "La planta alienígena absorbe agua con avidez." << std::endl;
    m_watered = true;
}

bool FakePlant::react() const {
    bool success = false;
    if (!m_watered) {
        std::cout << "La planta alienígena se encoge y evita el contacto." << std::endl;
    }
    else {
        std::cout << "La planta alienígena vibra y abre sus pétalos." << std::endl;
        success = true;
    }
    return success;
}

FakeRepairKit::FakeRepairKit()
    : m_ready(true) {
    std::cout << "Personaje: Un kit de reparación se abre con recambios ordenados." << std::endl;
}

FakeRepairKit::~FakeRepairKit() {
    std::cout << "Personaje: El kit de reparación se cierra y se aparta." << std::endl;
}

void FakeRepairKit::restock() {
    std::cout << "El kit de reparación recupera consumibles y recambios." << std::endl;
    m_ready = true;
}

bool FakeRepairKit::repair() {
    bool success = false;
    if (!m_ready) {
        std::cout << "El kit de reparación está vacío y no puede actuar." << std::endl;
    }
    else {
        std::cout << "El kit de reparación resuelve la avería en pocos pasos." << std::endl;
        m_ready = false;
        success = true;
    }
    return success;
}

FakeRobotDrone::FakeRobotDrone()
    : m_active(false) {
    std::cout << "Personaje: Un dron auxiliar espera instrucciones sobre su base." << std::endl;
}

FakeRobotDrone::~FakeRobotDrone() {
    std::cout << "Personaje: El dron auxiliar aterriza y se apaga." << std::endl;
}

void FakeRobotDrone::activate() {
    std::cout << "El dron auxiliar enciende sus rotores." << std::endl;
    m_active = true;
}

void FakeRobotDrone::deactivate() {
    std::cout << "El dron auxiliar corta motores y queda estable." << std::endl;
    m_active = false;
}

bool FakeRobotDrone::assist() const {
    bool success = false;
    if (!m_active) {
        std::cout << "El dron auxiliar no puede asistir en reposo." << std::endl;
    }
    else {
        std::cout << "El dron auxiliar completa una tarea simple." << std::endl;
        success = true;
    }
    return success;
}

FakeRubberDuck::FakeRubberDuck() {
    std::cout << "Personaje: Un pato de goma aparece donde menos se espera." << std::endl;
}

FakeRubberDuck::~FakeRubberDuck() {
    std::cout << "Personaje: El pato de goma flota fuera de cuadro." << std::endl;
}

void FakeRubberDuck::squeak() const {
    std::cout << "El pato de goma suelta un chirrido alegre." << std::endl;
}

void FakeRubberDuck::drift() const {
    std::cout << "El pato de goma se desplaza sin prisa por el set." << std::endl;
}

FakeSalvager::FakeSalvager() {
    std::cout << "Personaje: Una chatarrera examina restos junto a un casco oxidado." << std::endl;
}

FakeSalvager::~FakeSalvager() {
    std::cout << "Personaje: La chatarrera se retira con un saco de piezas útiles." << std::endl;
}

bool FakeSalvager::recover(FakeEnergyCell& cell) const {
    bool success = cell.use();
    if (!success) {
        cell.recharge();
        success = cell.use();
    }

    if (!success) {
        std::cout << "La chatarrera no logra extraer energía aprovechable." << std::endl;
    }
    else {
        std::cout << "La chatarrera recupera una carga valiosa de la célula." << std::endl;
    }
    return success;
}

bool FakeSalvager::collect(FakeToolbox& toolbox) const {
    toolbox.open();
    bool success = toolbox.has_tools();
    if (!success) {
        std::cout << "La chatarrera no encuentra piezas reutilizables." << std::endl;
    }
    else {
        std::cout << "La chatarrera guarda varias herramientas todavía funcionales." << std::endl;
    }
    return success;
}

void FakeSalvager::search() const {
    std::cout << "La chatarrera peina cada rincón en busca de valor oculto." << std::endl;
}

FakeScanner::FakeScanner()
    : m_calibrated(false) {
    std::cout << "Personaje: Un escáner se activa con un pulso de diagnóstico." << std::endl;
}

FakeScanner::~FakeScanner() {
    std::cout << "Personaje: El escáner vuelve a su funda." << std::endl;
}

void FakeScanner::calibrate() {
    std::cout << "El escáner ajusta sensibilidad y rango." << std::endl;
    m_calibrated = true;
}

bool FakeScanner::detect_signal() const {
    bool success = false;
    if (!m_calibrated) {
        std::cout << "El escáner no interpreta señales sin calibrar." << std::endl;
    }
    else {
        std::cout << "El escáner detecta un pulso tenue a distancia." << std::endl;
        success = true;
    }
    return success;
}

FakeScientist::FakeScientist() {
    std::cout << "Personaje: Una científica prepara un escáner portátil." << std::endl;
}

FakeScientist::~FakeScientist() {
    std::cout << "Personaje: La científica apaga su equipo de análisis." << std::endl;
}

bool FakeScientist::analyze(const FakeCrystal& crystal) const {
    bool success = crystal.is_glowing();
    if (!success) {
        std::cout << "La científica no detecta actividad relevante en la muestra." << std::endl;
    }
    else {
        std::cout << "La científica confirma una firma energética poco común." << std::endl;
    }
    return success;
}

bool FakeScientist::scan(const FakeStrangeArtifact& artifact) const {
    bool success = artifact.is_awake();
    if (!success) {
        std::cout << "La científica solo registra ruido de fondo." << std::endl;
    }
    else {
        std::cout << "La científica obtiene un patrón de lectura estable." << std::endl;
    }
    return success;
}

void FakeScientist::write(FakeDataPad& pad) const {
    std::cout << "La científica documenta cada hallazgo sin saltar detalles." << std::endl;
    pad.write_note();
}

FakeSmuggler::FakeSmuggler() {
    std::cout << "Personaje: Una contrabandista aparece con capucha y paso ligero." << std::endl;
}

FakeSmuggler::~FakeSmuggler() {
    std::cout << "Personaje: La contrabandista se esfuma por un pasillo secundario." << std::endl;
}

bool FakeSmuggler::hide(FakeCargoCrate& crate) const {
    crate.open();
    bool success = crate.is_open();
    if (!success) {
        std::cout << "La contrabandista no encuentra dónde ocultar la mercancía." << std::endl;
    }
    else {
        std::cout << "La contrabandista oculta el cargamento sin dejar rastro." << std::endl;
        crate.close();
    }
    return success;
}

bool FakeSmuggler::escape(FakeSpaceship& ship) const {
    ship.set_course();
    ship.launch();
    bool success = ship.travel();
    if (!success) {
        std::cout << "La contrabandista cancela la fuga ante demasiado riesgo." << std::endl;
    }
    else {
        std::cout << "La contrabandista toma una ruta secundaria y desaparece." << std::endl;
    }
    return success;
}

void FakeSmuggler::sneak() const {
    std::cout << "La contrabandista avanza sin hacer ruido ni levantar miradas." << std::endl;
}

FakeSpaceFarmer::FakeSpaceFarmer() {
    std::cout << "Personaje: Una granjera espacial revisa domos de cultivo." << std::endl;
}

FakeSpaceFarmer::~FakeSpaceFarmer() {
    std::cout << "Personaje: La granjera espacial cierra riego y luces de crecimiento." << std::endl;
}

bool FakeSpaceFarmer::water(FakePlant& plant) const {
    plant.water();
    bool success = plant.react();
    if (!success) {
        std::cout << "La granjera ajusta nutrientes al ver una respuesta débil." << std::endl;
    }
    else {
        std::cout << "La granjera confirma que el cultivo responde al riego." << std::endl;
    }
    return success;
}

bool FakeSpaceFarmer::harvest(FakePlant& plant) const {
    bool success = plant.react();
    if (!success) {
        std::cout << "La granjera deja madurar el cultivo un ciclo más." << std::endl;
    }
    else {
        std::cout << "La granjera recoge la cosecha con movimientos precisos." << std::endl;
    }
    return success;
}

void FakeSpaceFarmer::plantSeed() const {
    std::cout << "La granjera deposita una nueva semilla en el sustrato." << std::endl;
}

FakeSpaceSuit::FakeSpaceSuit()
    : m_sealed(false) {
    std::cout << "Personaje: Un traje espacial queda preparado junto a la esclusa." << std::endl;
}

FakeSpaceSuit::~FakeSpaceSuit() {
    std::cout << "Personaje: El traje espacial vuelve a su soporte." << std::endl;
}

void FakeSpaceSuit::seal() {
    std::cout << "El traje espacial cierra juntas y presión." << std::endl;
    m_sealed = true;
}

void FakeSpaceSuit::unseal() {
    std::cout << "El traje espacial libera el cierre de seguridad." << std::endl;
    m_sealed = false;
}

bool FakeSpaceSuit::protects() const {
    bool success = false;
    if (!m_sealed) {
        std::cout << "El traje espacial no está listo para el exterior." << std::endl;
    }
    else {
        std::cout << "El traje espacial mantiene un aislamiento completo." << std::endl;
        success = true;
    }
    return success;
}

FakeSpaceTourist::FakeSpaceTourist() {
    std::cout << "Personaje: Una turista espacial observa todo con fascinación." << std::endl;
}

FakeSpaceTourist::~FakeSpaceTourist() {
    std::cout << "Personaje: La turista espacial se aleja con recuerdos nuevos." << std::endl;
}

bool FakeSpaceTourist::takePhoto(const FakeStrangeArtifact& artifact) const {
    bool success = artifact.is_awake();
    if (!success) {
        std::cout << "La foto sale apagada y sin detalle." << std::endl;
    }
    else {
        std::cout << "La foto captura un brillo que nadie esperaba." << std::endl;
    }
    return success;
}

void FakeSpaceTourist::buy(FakeMerchant& merchant) const {
    FakeCoin coin;
    bool success = merchant.sell(coin);
    if (!success) {
        std::cout << "La turista duda y guarda la cartera." << std::endl;
    }
    else {
        std::cout << "La turista paga encantada por un recuerdo singular." << std::endl;
    }
}

void FakeSpaceTourist::wave() const {
    std::cout << "La turista saluda a todo el mundo con entusiasmo." << std::endl;
}

FakeSpaceship::FakeSpaceship()
    : m_in_flight(false), m_has_course(false) {
    std::cout << "Personaje: Una nave aterriza en el hangar." << std::endl;
}

FakeSpaceship::~FakeSpaceship() {
    std::cout << "Personaje: La nave queda fuera de plano." << std::endl;
}

void FakeSpaceship::set_course() {
    std::cout << "- La nave dice 'Beep boop'." << std::endl;
    m_has_course = true;
}

void FakeSpaceship::launch() {
    if (!m_has_course) {
        std::cout << "La nave mantiene motores al mínimo, sin rumbo marcado." << std::endl;
    }
    else {
        std::cout << "La nave despega con un rugido suave." << std::endl;
        m_in_flight = true;
    }
}

void FakeSpaceship::dock() {
    if (!m_in_flight) {
        std::cout << "La nave ya está acoplada al muelle." << std::endl;
    }
    else {
        std::cout << "La nave completa un acoplamiento limpio." << std::endl;
        m_in_flight = false;
    }
}

bool FakeSpaceship::travel() const {
    bool success = false;
    if (!m_in_flight) {
        std::cout << "La nave permanece quieta, a la espera de salida." << std::endl;
    }
    else {
        std::cout << "La nave cruza la ruta marcada sin contratiempos." << std::endl;
        success = true;
    }
    return success;
}

FakeStarMap::FakeStarMap()
    : m_open(false) {
    std::cout << "Personaje: Un mapa estelar aparece plegado sobre la consola." << std::endl;
}

FakeStarMap::~FakeStarMap() {
    std::cout << "Personaje: El mapa estelar se guarda en su funda." << std::endl;
}

void FakeStarMap::unfold() {
    std::cout << "El mapa estelar despliega rutas entre sistemas." << std::endl;
    m_open = true;
}

void FakeStarMap::fold() {
    std::cout << "El mapa estelar vuelve a quedar plegado." << std::endl;
    m_open = false;
}

bool FakeStarMap::show_route() const {
    bool success = false;
    if (!m_open) {
        std::cout << "El mapa estelar no muestra rutas al estar cerrado." << std::endl;
    }
    else {
        std::cout << "El mapa estelar resalta un corredor seguro." << std::endl;
        success = true;
    }
    return success;
}

FakeStrangeArtifact::FakeStrangeArtifact()
    : m_awake(false) {
    std::cout << "Personaje: Un artefacto extraño descansa en silencio." << std::endl;
}

FakeStrangeArtifact::~FakeStrangeArtifact() {
    std::cout << "Personaje: El artefacto extraño desaparece entre sombras." << std::endl;
}

void FakeStrangeArtifact::awaken() {
    std::cout << "El artefacto extraño vibra con energía antigua." << std::endl;
    m_awake = true;
}

void FakeStrangeArtifact::rest() {
    std::cout << "El artefacto extraño apaga su pulso luminoso." << std::endl;
    m_awake = false;
}

bool FakeStrangeArtifact::is_awake() const {
    return m_awake;
}

FakeStreetVendor::FakeStreetVendor()
    : m_has_stock(true) {
    std::cout << "Personaje: Un vendedor ambulante despliega gadgets sobre una manta." << std::endl;
}

FakeStreetVendor::~FakeStreetVendor() {
    std::cout << "Personaje: El vendedor ambulante recoge el puesto a toda prisa." << std::endl;
}

bool FakeStreetVendor::sell() {
    bool success = false;
    if (!m_has_stock) {
        std::cout << "El vendedor anuncia oferta, pero ya no le queda género." << std::endl;
    }
    else {
        std::cout << "El vendedor coloca un artilugio raro en manos del cliente." << std::endl;
        m_has_stock = false;
        success = true;
    }
    return success;
}

void FakeStreetVendor::shout() const {
    std::cout << "El vendedor grita gangas imposibles de ignorar." << std::endl;
}

bool FakeStreetVendor::accept(FakeCoin& coin) {
    bool success = coin.spend();
    if (!success) {
        std::cout << "El vendedor devuelve la moneda y niega la venta." << std::endl;
    }
    else {
        std::cout << "El vendedor acepta el pago y repone parte del puesto." << std::endl;
        m_has_stock = true;
    }
    return success;
}

FakeToolbox::FakeToolbox()
    : m_open(false) {
    std::cout << "Personaje: Una caja de herramientas aparece lista para intervenir." << std::endl;
}

FakeToolbox::~FakeToolbox() {
    std::cout << "Personaje: La caja de herramientas sale de escena." << std::endl;
}

void FakeToolbox::open() {
    std::cout << "La caja de herramientas abre todos sus compartimentos." << std::endl;
    m_open = true;
}

void FakeToolbox::close() {
    std::cout << "La caja de herramientas vuelve a cerrarse." << std::endl;
    m_open = false;
}

bool FakeToolbox::has_tools() const {
    bool success = false;
    if (!m_open) {
        std::cout << "La caja de herramientas sigue cerrada." << std::endl;
    }
    else {
        std::cout << "La caja de herramientas ofrece piezas para una reparación rápida." << std::endl;
        success = true;
    }
    return success;
}

FakeToolboxRobot::FakeToolboxRobot()
    : m_deployed(false) {
    std::cout << "Personaje: Un robot de herramientas sale del taller en modo espera." << std::endl;
}

FakeToolboxRobot::~FakeToolboxRobot() {
    std::cout << "Personaje: El robot de herramientas vuelve al taller." << std::endl;
}

void FakeToolboxRobot::deploy() {
    std::cout << "El robot de herramientas despliega brazos de trabajo." << std::endl;
    m_deployed = true;
}

void FakeToolboxRobot::dock() {
    std::cout << "El robot de herramientas pliega brazos y se acopla." << std::endl;
    m_deployed = false;
}

bool FakeToolboxRobot::hand_tool() const {
    bool success = false;
    if (!m_deployed) {
        std::cout << "El robot de herramientas no entrega piezas en reposo." << std::endl;
    }
    else {
        std::cout << "El robot de herramientas acerca una llave calibrada." << std::endl;
        success = true;
    }
    return success;
}

void FakeTree::blow() const {
    std::cout << "Un árbol agita sus ramas mecido por el viento." << std::endl;
}

FakeVendingMachine::FakeVendingMachine()
    : m_has_credit(false) {
    std::cout << "Personaje: Una máquina expendedora enciende su panel luminoso." << std::endl;
}

FakeVendingMachine::~FakeVendingMachine() {
    std::cout << "Personaje: La máquina expendedora baja sus luces." << std::endl;
}

void FakeVendingMachine::insert_coin() {
    std::cout << "La máquina expendedora reconoce una moneda universal." << std::endl;
    m_has_credit = true;
}

bool FakeVendingMachine::dispense() {
    bool success = false;
    if (!m_has_credit) {
        std::cout << "La máquina expendedora no entrega producto sin saldo." << std::endl;
    }
    else {
        std::cout << "La máquina expendedora libera un paquete inesperado." << std::endl;
        m_has_credit = false;
        success = true;
    }
    return success;
}
