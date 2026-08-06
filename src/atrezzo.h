/**
 * Fake objects and characters for fiction scenes.
 * Instead of actual effects, messages are shown via std::cout.
 */
#pragma once

/// An access card for restricted doors.
class FakeAccessCard;

/// A strange visitor with unusual social habits.
class FakeAlien;

/// A tiny companion that follows alien owners.
class FakeAlienPet;

/// A humanoid machine with practical skills.
class FakeAndroid;

/// A curator preserving records and relics.
class FakeArchivist;

/// A space traveler focused on discovery.
class FakeAstronaut;

/// A backpack for carrying expedition gear.
class FakeBackpack;

/// A standard laser sidearm prop.
class FakeBlaster;

/// A tracker specialized in difficult targets.
class FakeBountyHunter;

/// An official handling forms and approvals.
class FakeBureaucrat;

/// A crew leader coordinating ship operations.
class FakeCaptain;

/// A cargo container for supplies and loot.
class FakeCargoCrate;

/// A coin used as universal currency.
class FakeCoin;

/// A communication device for distant calls.
class FakeCommunicator;

/// A courier carrying critical deliveries.
class FakeCourier;

/// A glowing crystal with unusual properties.
class FakeCrystal;

/// A handheld pad for notes and records.
class FakeDataPad;

/// An investigator with sharp instincts and clumsy timing.
class FakeDetective;

/// A negotiator between crews, factions, and species.
class FakeDiplomat;

/// A fake dog with feed and pet interactions.
class FakeDog;

/// A portable power source for equipment.
class FakeEnergyCell;

/// An adventurer searching new places and clues.
class FakeExplorer;

/// A showman predicting futures with confidence.
class FakeFortuneTeller;

/// Boots that improve traction on unusual surfaces.
class FakeGravityBoots;

/// A space helmet used with protective gear.
class FakeHelmet;

/// A projector for maps, messages, and holograms.
class FakeHologramProjector;

/// A builder of practical and odd inventions.
class FakeInventor;

/// A quiet keeper of clean corridors and hidden clues.
class FakeJanitor;

/// A compact jetpack for short flights.
class FakeJetpack;

/// A repair specialist for ships and drones.
class FakeMechanic;

/// A trader dealing in common and odd goods.
class FakeMerchant;

/// A worker extracting resources from harsh terrain.
class FakeMiner;

/// A spacecraft specialist for routes and landings.
class FakePilot;

/// An alien plant that reacts to nearby changes.
class FakePlant;

/// A quick kit for emergency fixes.
class FakeRepairKit;

/// A small drone for simple assistance tasks.
class FakeRobotDrone;

/// A rubber duck that appears in unlikely places.
class FakeRubberDuck;

/// A scavenger focused on reusable parts.
class FakeSalvager;

/// A scanner for signals and nearby anomalies.
class FakeScanner;

/// A researcher focused on anomalies and evidence.
class FakeScientist;

/// A covert operator using hidden routes.
class FakeSmuggler;

/// A grower managing food and plants in controlled habitats.
class FakeSpaceFarmer;

/// A protective suit for dangerous environments.
class FakeSpaceSuit;

/// A casual traveler visiting famous landmarks.
class FakeSpaceTourist;

/// A reusable spaceship prop for travel scenes.
class FakeSpaceship;

/// A map that contains routes between worlds.
class FakeStarMap;

/// A mysterious relic with unknown behavior.
class FakeStrangeArtifact;

/// A noisy vendor offering quick deals.
class FakeStreetVendor;

/// A toolbox that keeps repair tools ready.
class FakeToolbox;

/// A tiny robot specialized in carrying tools.
class FakeToolboxRobot;

/// A fake tree for scene atmosphere.
class FakeTree;

/// A vending machine for snacks and surprises.
class FakeVendingMachine;


class FakeAccessCard {
public:
    /// Create an inactive access card.
    FakeAccessCard();
    /// Remove the access card from the scene.
    ~FakeAccessCard();

    /// Activate the card credentials.
    void activate();

    /// Try to open a secure door.
    /// @return True when the active card unlocks the door.
    bool unlock() const;

private:
    /// Whether the card credentials are active.
    bool m_active;
};

class FakeAlien {
public:
    /// Bring an alien into the scene.
    FakeAlien();
    /// Remove the alien from the scene.
    ~FakeAlien();

    /// Greet an astronaut.
    /// @param astronaut Astronaut receiving the greeting.
    void greet(const FakeAstronaut& astronaut) const;

    /// Try to trade with a merchant.
    /// @param merchant Merchant involved in the trade.
    /// @return True when the exchange is accepted.
    bool trade(FakeMerchant& merchant) const;

    /// Try to pet an alien pet.
    /// @param pet Creature being petted.
    /// @return True when the pet allows contact.
    bool pet(FakeAlienPet& pet) const;
};

class FakeAlienPet {
public:
    /// Bring an alien pet into the scene.
    FakeAlienPet();
    /// Remove the alien pet from the scene.
    ~FakeAlienPet();

    /// Follow an alien owner.
    /// @param owner Alien that leads the pet.
    void follow(const FakeAlien& owner);

    /// Try to eat from a plant.
    /// @param plant Plant used as food source.
    /// @return True when the pet manages to eat.
    bool eat(FakePlant& plant);

    /// Hide from nearby activity.
    void hide();

private:
    /// Whether the pet is currently hidden.
    bool m_hidden;
};

class FakeAndroid {
public:
    /// Bring an android into the scene.
    FakeAndroid();
    /// Remove the android from the scene.
    ~FakeAndroid();

    /// Perform a quick calculation.
    /// @return True when the computation resolves.
    bool calculate() const;

    /// Assist a scientist.
    /// @param scientist Scientist receiving support.
    void assist(const FakeScientist& scientist) const;

    /// Recharge using an energy cell.
    /// @param cell Energy cell used as power source.
    /// @return True when recharge succeeds.
    bool recharge(FakeEnergyCell& cell) const;
};

class FakeArchivist {
public:
    /// Bring an archivist into the scene.
    FakeArchivist();
    /// Remove the archivist from the scene.
    ~FakeArchivist();

    /// Record an artifact entry.
    /// @param artifact Artifact to catalog.
    /// @return True when the record is completed.
    bool record(FakeStrangeArtifact& artifact) const;

    /// Search notes on a data pad.
    /// @param pad Pad used for lookup.
    /// @return True when relevant notes are found.
    bool search(const FakeDataPad& pad) const;

    /// Organize archived material.
    void organize() const;
};

class FakeAstronaut {
public:
    /// Bring an astronaut into the scene.
    FakeAstronaut();
    /// Remove the astronaut from the scene.
    ~FakeAstronaut();

    /// Try to explore using a spaceship route.
    /// @param ship Spaceship used for the exploration.
    /// @return True when exploration can proceed.
    bool explore(FakeSpaceship& ship) const;

    /// Try to collect a strange artifact.
    /// @param artifact Artifact targeted for collection.
    /// @return True when the artifact is active enough to collect.
    bool collect(FakeStrangeArtifact& artifact) const;

    /// Wave to an alien visitor.
    /// @param alien Alien that receives the gesture.
    void wave(const FakeAlien& alien) const;
};

class FakeBackpack {
public:
    /// Create an unpacked backpack.
    FakeBackpack();
    /// Remove the backpack from the scene.
    ~FakeBackpack();

    /// Pack the backpack.
    void pack();
    /// Unpack the backpack.
    void unpack();
    /// Return whether the backpack is currently packed.
    /// @return True when the backpack is packed.
    bool is_packed() const;

private:
    /// Whether the backpack is currently packed.
    bool m_packed;
};

class FakeBlaster {
public:
    /// Create an uncharged blaster.
    FakeBlaster();
    /// Remove the blaster from the scene.
    ~FakeBlaster();

    /// Charge the blaster.
    void charge();

    /// Fire the blaster.
    /// @return True when a charged shot is fired.
    bool fire();

private:
    /// Whether the blaster is charged.
    bool m_charged;
};

class FakeBountyHunter {
public:
    /// Bring a bounty hunter into the scene.
    FakeBountyHunter();
    /// Remove the bounty hunter from the scene.
    ~FakeBountyHunter();

    /// Chase a smuggler.
    /// @param smuggler Target attempting escape.
    /// @return True when the target is pressured.
    bool chase(FakeSmuggler& smuggler) const;

    /// Capture an alien.
    /// @param alien Target to capture.
    /// @return True when the alien is cornered.
    bool capture(FakeAlien& alien) const;

    /// Aim a blaster before a shot.
    /// @param blaster Blaster used for aiming.
    void aim(FakeBlaster& blaster) const;
};

class FakeBureaucrat {
public:
    /// Bring a bureaucrat into the scene.
    FakeBureaucrat();
    /// Remove the bureaucrat from the scene.
    ~FakeBureaucrat();

    /// Approve a form.
    /// @param form Form under review.
    /// @return True when approval is granted.
    bool approve(FakeDataPad& form) const;

    /// Reject a form.
    /// @param form Form under review.
    /// @return True when rejection is issued.
    bool reject(FakeDataPad& form) const;

    /// Stamp official paperwork.
    void stamp() const;
};

class FakeCaptain {
public:
    /// Bring a captain into the scene.
    FakeCaptain();
    /// Remove the captain from the scene.
    ~FakeCaptain();

    /// Give an order to the pilot.
    /// @param pilot Pilot receiving the order.
    void giveOrder(FakePilot& pilot) const;

    /// Try to board a spaceship.
    /// @param ship Spaceship to board.
    /// @return True when the captain can board safely.
    bool board(FakeSpaceship& ship) const;

    /// Inspect a cargo crate.
    /// @param crate Cargo crate under inspection.
    /// @return True when the crate appears accessible.
    bool inspect(const FakeCargoCrate& crate) const;
};

class FakeCargoCrate {
public:
    /// Create a closed cargo crate.
    FakeCargoCrate();
    /// Remove the cargo crate from the scene.
    ~FakeCargoCrate();

    /// Open the crate.
    void open();

    /// Close the crate.
    void close();

    /// Return whether the crate is currently open.
    /// @return True when the crate is open.
    bool is_open() const;

private:
    /// Whether the crate is currently open.
    bool m_open;
};

class FakeCoin {
public:
    /// Create an unspent coin.
    FakeCoin();
    /// Remove the coin from the scene.
    ~FakeCoin();

    /// Polish the coin for display.
    void polish() const;

    /// Spend the coin.
    /// @return True when the coin was not spent before.
    bool spend();

private:
    /// Whether the coin has already been spent.
    bool m_present;
};

class FakeCommunicator {
public:
    /// Create an offline communicator.
    FakeCommunicator();
    /// Remove the communicator from the scene.
    ~FakeCommunicator();

    /// Connect the communicator to a channel.
    void tune();

    /// Place a call.
    /// @return True when the communicator is online.
    bool call() const;

private:
    /// Whether the communicator is online.
    bool m_online;
};

class FakeCourier {
public:
    /// Bring a courier into the scene.
    FakeCourier();
    /// Remove the courier from the scene.
    ~FakeCourier();

    /// Deliver a package crate.
    /// @param crate Crate being delivered.
    /// @return True when the courier was carrying cargo.
    bool deliver(FakeCargoCrate& crate);

    /// Receive a package crate.
    /// @param crate Crate being received.
    /// @return True when the pickup succeeds.
    bool receive(FakeCargoCrate& crate);

    /// Keep moving fast.
    void hurry() const;

private:
    /// Whether the courier is carrying a delivery.
    bool m_carrying_package;
};

class FakeCrystal {
public:
    /// Create a dim crystal.
    FakeCrystal();
    /// Remove the crystal from the scene.
    ~FakeCrystal();

    /// Make the crystal glow.
    void charge();

    /// Dim the crystal glow.
    void dim();

    /// Return whether the crystal is currently glowing.
    /// @return True when the crystal is glowing.
    bool is_glowing() const;

private:
    /// Whether the crystal is currently glowing.
    bool m_glowing;
};

class FakeDataPad {
public:
    /// Create an empty data pad.
    FakeDataPad();
    /// Remove the data pad from the scene.
    ~FakeDataPad();

    /// Write a quick note.
    void write_note();

    /// Read the note.
    /// @return True when a note is available.
    bool read_note() const;

private:
    /// Whether the data pad currently stores a note.
    bool m_has_note;
};

class FakeDetective {
public:
    /// Bring a detective into the scene.
    FakeDetective();
    /// Remove the detective from the scene.
    ~FakeDetective();

    /// Investigate a suspicious clue.
    /// @return True when the investigation stays on track.
    bool investigate() const;

    /// Inspect an artifact in detail.
    /// @param artifact Artifact under inspection.
    /// @return True when something notable is found.
    bool inspect(const FakeStrangeArtifact& artifact) const;

    /// Stumble at the worst possible moment.
    void trip();

private:
    /// Whether the detective is currently steady on their feet.
    bool m_balanced;
};

class FakeDiplomat {
public:
    /// Bring a diplomat into the scene.
    FakeDiplomat();
    /// Remove the diplomat from the scene.
    ~FakeDiplomat();

    /// Negotiate with an alien representative.
    /// @param alien Alien counterpart in talks.
    /// @return True when the tone stays constructive.
    bool negotiate(FakeAlien& alien) const;

    /// Greet a captain with protocol.
    /// @param captain Captain receiving the greeting.
    void greet(const FakeCaptain& captain) const;

    /// Sign an agreement on a data pad.
    /// @param pad Pad containing the agreement.
    /// @return True when signing can proceed.
    bool sign(FakeDataPad& pad) const;
};

class FakeDog {
public:
    /// Create a dog that starts unfriendly.
    FakeDog();
    /// Remove the dog from the scene.
    ~FakeDog();

    /// Feed the dog and make it friendly.
    void feed();

    /// Try to pet the dog.
    /// @return True when the dog allows petting.
    bool pet() const;

private:
    /// Whether the dog currently allows petting.
    bool m_friendly;
};

class FakeEnergyCell {
public:
    /// Create a charged energy cell.
    FakeEnergyCell();
    /// Remove the energy cell from the scene.
    ~FakeEnergyCell();

    /// Recharge the cell.
    void recharge();

    /// Consume one charge.
    /// @return True when the cell had charge available.
    bool use();

private:
    /// Whether the cell currently has usable energy.
    bool m_charged;
};

class FakeExplorer {
public:
    /// Bring an explorer into the scene.
    FakeExplorer();
    /// Remove the explorer from the scene.
    ~FakeExplorer();

    /// Try to discover an artifact.
    /// @param artifact Artifact that may reveal itself.
    /// @return True when the discovery succeeds.
    bool discover(FakeStrangeArtifact& artifact) const;

    /// Follow a mapped route.
    /// @param map Map used to continue the journey.
    void follow(const FakeStarMap& map) const;

    /// Try to climb an alien plant.
    /// @param plant Plant used as support.
    /// @return True when the plant reacts favorably.
    bool climb(FakePlant& plant) const;
};

class FakeFortuneTeller {
public:
    /// Bring a fortune teller into the scene.
    FakeFortuneTeller();
    /// Remove the fortune teller from the scene.
    ~FakeFortuneTeller();

    /// Predict a path for an astronaut.
    /// @param astronaut Person receiving the prediction.
    /// @return True when the prediction sounds convincing.
    bool predict(const FakeAstronaut& astronaut) const;

    /// Read patterns from a crystal.
    /// @param crystal Crystal used for interpretation.
    void read(FakeCrystal& crystal) const;

    /// Smile mysteriously.
    void smile() const;
};

class FakeGravityBoots {
public:
    /// Create disabled gravity boots.
    FakeGravityBoots();
    /// Remove the gravity boots from the scene.
    ~FakeGravityBoots();

    /// Enable the gravity field.
    void enable();

    /// Disable the gravity field.
    void disable();

    /// Return whether the boots can keep stable footing.
    /// @return True when gravity control is enabled.
    bool walk_any_surface() const;

private:
    /// Whether gravity control is enabled.
    bool m_enabled;
};

class FakeHelmet {
public:
    /// Create a helmet that is not being worn.
    FakeHelmet();
    /// Remove the helmet from the scene.
    ~FakeHelmet();

    /// Put the helmet on.
    void wear();

    /// Remove the helmet.
    void remove();

    /// Return whether the helmet is currently worn.
    /// @return True when the helmet is worn.
    bool is_worn() const;

private:
    /// Whether the helmet is currently worn.
    bool m_worn;
};

class FakeHologramProjector {
public:
    /// Create a ready hologram projector.
    FakeHologramProjector();
    /// Remove the hologram projector from the scene.
    ~FakeHologramProjector();

    /// Project a route map.
    void project_map() const;

    /// Project a floating message.
    void project_message() const;

    /// Clear the current hologram.
    void clear() const;
};

class FakeInventor {
public:
    /// Bring an inventor into the scene.
    FakeInventor();
    /// Remove the inventor from the scene.
    ~FakeInventor();

    /// Build or rebuild a robot drone.
    /// @param drone Drone being assembled.
    /// @return True when activation succeeds.
    bool build(FakeRobotDrone& drone) const;

    /// Upgrade a communicator device.
    /// @param device Communicator being upgraded.
    /// @return True when the device comes online.
    bool upgrade(FakeCommunicator& device) const;

    /// Run a quick field test.
    void test() const;
};

class FakeJanitor {
public:
    /// Bring a janitor into the scene.
    FakeJanitor();
    /// Remove the janitor from the scene.
    ~FakeJanitor();

    /// Clean around a spaceship bay.
    /// @param ship Spaceship area being cleaned.
    void clean(FakeSpaceship& ship) const;

    /// Find a lost coin.
    /// @param coin Coin discovered during cleaning.
    /// @return True when the coin is still spendable.
    bool find(FakeCoin& coin) const;

    /// Whistle while working.
    void whistle() const;
};

class FakeJetpack {
public:
    /// Create an inactive jetpack.
    FakeJetpack();
    /// Remove the jetpack from the scene.
    ~FakeJetpack();

    /// Start the jetpack engines.
    void ignite();

    /// Stop the jetpack engines.
    void shutdown();

    /// Show whether the jetpack can boost right now.
    /// @return True when the jetpack is active.
    bool boost() const;

private:
    /// Whether the jetpack is currently active.
    bool m_active;
};

class FakeMechanic {
public:
    /// Bring a mechanic into the scene.
    FakeMechanic();
    /// Remove the mechanic from the scene.
    ~FakeMechanic();

    /// Try to repair a spaceship.
    /// @param ship Spaceship to repair.
    /// @return True when the repair is feasible.
    bool repair(FakeSpaceship& ship) const;
    /// Try to repair a robot drone.
    /// @param drone Drone to repair.
    /// @return True when the drone responds after repair.
    bool repair(FakeRobotDrone& drone) const;

    /// Use a toolbox for maintenance.
    /// @param toolbox Toolbox used during the task.
    void use(const FakeToolbox& toolbox) const;
};

class FakeMerchant {
public:
    /// Bring a merchant into the scene.
    FakeMerchant();
    /// Remove the merchant from the scene.
    ~FakeMerchant();

    /// Sell an item in exchange for a coin.
    /// @param coin Coin used for payment.
    /// @return True when payment is valid.
    bool sell(FakeCoin& coin) const;

    /// Buy a strange artifact.
    /// @param artifact Artifact offered for sale.
    /// @return True when the merchant agrees to buy.
    bool buy(FakeStrangeArtifact& artifact) const;

    /// Negotiate terms with a diplomat.
    /// @param diplomat Diplomat taking part in the talk.
    void negotiate(FakeDiplomat& diplomat) const;
};

class FakeMiner {
public:
    /// Bring a miner into the scene.
    FakeMiner();
    /// Remove the miner from the scene.
    ~FakeMiner();

    /// Mine energy from a crystal.
    /// @param crystal Crystal used for extraction.
    /// @return True when extraction yields output.
    bool mine(FakeCrystal& crystal) const;

    /// Load a crate with mined material.
    /// @param crate Crate used for loading.
    /// @return True when the crate is ready to carry cargo.
    bool load(FakeCargoCrate& crate) const;

    /// Dig through rough ground.
    void dig() const;
};

class FakePilot {
public:
    /// Bring a pilot into the scene.
    FakePilot();
    /// Remove the pilot from the scene.
    ~FakePilot();

    /// Try to fly a spaceship.
    /// @param ship Spaceship to fly.
    /// @return True when takeoff and travel succeed.
    bool fly(FakeSpaceship& ship) const;

    /// Try to land a spaceship.
    /// @param ship Spaceship to land.
    /// @return True when docking completes.
    bool land(FakeSpaceship& ship) const;

    /// Follow a star map route.
    /// @param map Map used to navigate.
    void follow(const FakeStarMap& map) const;
};

class FakePlant {
public:
    /// Create an unwatered plant.
    FakePlant();
    /// Remove the plant from the scene.
    ~FakePlant();

    /// Water the plant.
    void water();

    /// React to surroundings.
    /// @return True when the plant is hydrated.
    bool react() const;

private:
    /// Whether the plant was recently watered.
    bool m_watered;
};

class FakeRepairKit {
public:
    /// Create a stocked repair kit.
    FakeRepairKit();
    /// Remove the repair kit from the scene.
    ~FakeRepairKit();

    /// Restock the repair kit.
    void restock();
    /// Perform a repair.
    ///
    /// @return True when supplies are available.
    bool repair();

private:
    /// Whether the repair kit currently has supplies.
    bool m_ready;
};

class FakeRobotDrone {
public:
    /// Create an inactive drone.
    FakeRobotDrone();
    /// Remove the drone from the scene.
    ~FakeRobotDrone();

    /// Activate the drone systems.
    void activate();

    /// Deactivate the drone systems.
    void deactivate();

    /// Perform assistance.
    /// @return True when the drone is active.
    bool assist() const;

private:
    /// Whether the drone systems are active.
    bool m_active;
};

class FakeRubberDuck {
public:
    /// Create a rubber duck prop.
    FakeRubberDuck();
    /// Remove the rubber duck from the scene.
    ~FakeRubberDuck();

    /// Make the duck squeak.
    void squeak() const;
    /// Let the duck drift nearby.
    void drift() const;
};

class FakeSalvager {
public:
    /// Bring a salvager into the scene.
    FakeSalvager();
    /// Remove the salvager from the scene.
    ~FakeSalvager();

    /// Recover charge from an energy cell.
    /// @param cell Cell targeted for recovery.
    /// @return True when power can be extracted.
    bool recover(FakeEnergyCell& cell) const;

    /// Collect useful tools.
    /// @param toolbox Toolbox used for collection.
    /// @return True when tools are available.
    bool collect(FakeToolbox& toolbox) const;

    /// Search abandoned sections.
    void search() const;
};

class FakeScanner {
public:
    /// Create an uncalibrated scanner.
    FakeScanner();
    /// Remove the scanner from the scene.
    ~FakeScanner();

    /// Calibrate the scanner.
    void calibrate();

    /// Detect a signal.
    /// @return True when the scanner is calibrated.
    bool detect_signal() const;

private:
    /// Whether the scanner has been calibrated.
    bool m_calibrated;
};

class FakeScientist {
public:
    /// Bring a scientist into the scene.
    FakeScientist();
    /// Remove the scientist from the scene.
    ~FakeScientist();

    /// Analyze a crystal sample.
    /// @param crystal Crystal under analysis.
    /// @return True when the crystal is reactive.
    bool analyze(const FakeCrystal& crystal) const;

    /// Scan a strange artifact.
    /// @param artifact Artifact to scan.
    /// @return True when the artifact emits readable behavior.
    bool scan(const FakeStrangeArtifact& artifact) const;

    /// Write findings on a data pad.
    /// @param pad Data pad used for notes.
    void write(FakeDataPad& pad) const;
};

class FakeSmuggler {
public:
    /// Bring a smuggler into the scene.
    FakeSmuggler();
    /// Remove the smuggler from the scene.
    ~FakeSmuggler();

    /// Hide something inside a crate.
    /// @param crate Crate used to conceal cargo.
    /// @return True when the crate can be used.
    bool hide(FakeCargoCrate& crate) const;

    /// Escape aboard a spaceship.
    /// @param ship Spaceship used for escape.
    /// @return True when the escape route works.
    bool escape(FakeSpaceship& ship) const;

    /// Move without drawing attention.
    void sneak() const;
};

class FakeSpaceFarmer {
public:
    /// Bring a space farmer into the scene.
    FakeSpaceFarmer();
    /// Remove the space farmer from the scene.
    ~FakeSpaceFarmer();

    /// Water an alien plant.
    /// @param plant Plant receiving water.
    /// @return True when the plant responds to watering.
    bool water(FakePlant& plant) const;

    /// Harvest from an alien plant.
    /// @param plant Plant ready for harvest.
    /// @return True when harvest is possible.
    bool harvest(FakePlant& plant) const;

    /// Plant a new seed.
    void plantSeed() const;
};

class FakeSpaceSuit {
public:
    /// Create an unsealed space suit.
    FakeSpaceSuit();
    /// Remove the space suit from the scene.
    ~FakeSpaceSuit();

    /// Seal the suit for external conditions.
    void seal();

    /// Unseal the suit.
    void unseal();

    /// Show whether the suit is currently protective.
    /// @return True when the suit is sealed.
    bool protects() const;

private:
    /// Whether the suit is currently sealed.
    bool m_sealed;
};

class FakeSpaceTourist {
public:
    /// Bring a space tourist into the scene.
    FakeSpaceTourist();
    /// Remove the space tourist from the scene.
    ~FakeSpaceTourist();

    /// Try to take a photo of an artifact.
    /// @param artifact Artifact used as subject.
    /// @return True when the shot is worth keeping.
    bool takePhoto(const FakeStrangeArtifact& artifact) const;

    /// Buy something from a merchant.
    /// @param merchant Merchant involved in the purchase.
    void buy(FakeMerchant& merchant) const;

    /// Wave enthusiastically.
    void wave() const;
};

class FakeSpaceship {
public:
    /// Create a docked spaceship.
    FakeSpaceship();
    /// Remove the spaceship from the scene.
    ~FakeSpaceship();

    /// Set a destination before departure.
    void set_course();

    /// Launch the spaceship.
    void launch();

    /// Dock the spaceship.
    void dock();

    /// Show whether travel is currently possible.
    /// @return True when the spaceship is in flight.
    bool travel() const;

private:
    /// Whether the spaceship is currently in flight.
    bool m_in_flight;
    /// Whether the spaceship has a destination set.
    bool m_has_course;
};

class FakeStarMap {
public:
    /// Create a folded star map.
    FakeStarMap();
    /// Remove the star map from the scene.
    ~FakeStarMap();

    /// Fold the map.
    void fold();
    /// Unfold the map.
    void unfold();

    /// Show a route.
    /// @return True when the map is open.
    ///
    bool show_route() const;

private:
    /// Whether the map is currently open.
    bool m_open;
};

class FakeStrangeArtifact {
public:
    /// Create a dormant artifact.
    FakeStrangeArtifact();
    /// Remove the artifact from the scene.
    ~FakeStrangeArtifact();

    /// Wake the artifact behavior.
    void awaken();
    /// Return whether the artifact is currently active.
    /// @return True when the artifact is awake.
    bool is_awake() const;

    /// Calm the artifact behavior.
    void rest();

private:
    /// Whether the artifact is currently active.
    bool m_awake;
};

class FakeStreetVendor {
public:
    /// Bring a street vendor into the scene.
    FakeStreetVendor();
    /// Remove the street vendor from the scene.
    ~FakeStreetVendor();

    /// Try to sell a random item.
    /// @return True when stock is still available.
    bool sell();

    /// Shout promotional lines.
    void shout() const;

    /// Accept a coin from a buyer.
    /// @param coin Coin used for payment.
    /// @return True when the coin can be accepted.
    bool accept(FakeCoin& coin);

private:
    /// Whether the vendor still has stock to sell.
    bool m_has_stock;
};

class FakeToolbox {
public:
    /// Create a closed toolbox.
    FakeToolbox();
    /// Remove the toolbox from the scene.
    ~FakeToolbox();

    /// Open the toolbox.
    void open();

    /// Close the toolbox.
    void close();

    /// Show whether the toolbox is open and ready.
    /// @return True when the toolbox is open.
    bool has_tools() const;

private:
    /// Whether the toolbox is currently open.
    bool m_open;
};

class FakeToolboxRobot {
public:
    /// Create a docked toolbox robot.
    FakeToolboxRobot();
    /// Remove the toolbox robot from the scene.
    ~FakeToolboxRobot();

    /// Deploy the toolbox robot.
    void deploy();

    /// Dock the toolbox robot.
    void dock();

    /// Hand over a tool.
    /// @return True when the toolbox robot is deployed.
    bool hand_tool() const;

private:
    /// Whether the toolbox robot is currently deployed.
    bool m_deployed;
};

class FakeTree {
public:
    /// Show the tree moving its branches in the wind.
    void blow() const;
};

class FakeVendingMachine {
public:
    /// Create a vending machine without active credit.
    FakeVendingMachine();
    /// Remove the vending machine from the scene.
    ~FakeVendingMachine();

    /// Insert one credit.
    void insert_coin();

    /// Dispense an item.
    /// @return True when credit is available.
    bool dispense();

private:
    /// Whether the machine currently has credit.
    bool m_has_credit;
};
