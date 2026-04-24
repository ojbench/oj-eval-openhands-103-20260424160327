#ifndef GAME_H
#define GAME_H

#include <string>

struct Node;
struct GameState;
class Card;
class Pile;

/**
 * Skill card type
 * MULTIPLIER: Increases the score multiplier for the next score card
 * HEAD_BUTT: Moves the card at the bottom (tail) of the discard pile to the top (head) of the draw pile
 */
enum SkillCardType {
    MULTIPLIER,
    HEAD_BUTT,
};

/** 
 * Card base class
 * TODO: Add member variables and functions if needed
 */
class Card {
public:
    explicit Card() = default;
    virtual ~Card() = default;
    // Execute card effect, implemented by derived classes. You need to implement the specific effect of the current card type in each derived class
    virtual void play(GameState& game_state) = 0;
};

/** 
 * Score card class
 * TODO: Add member variables and functions as needed
 * You can modify the destructor if needed
 */
class ScoreCard: public Card {
private:
    int point_;
public:
    explicit ScoreCard(int point);
    void play(GameState &game_state) override;
    ~ScoreCard() override = default;
};

/** 
 * Skill card class
 * TODO: Add member variables and functions as needed
 * You can modify the destructor if needed
 */
class SkillCard: public Card {
private:
    SkillCardType type_;
public:
    explicit SkillCard(SkillCardType skill_card_type);
    void play(GameState &game_state) override;
    ~SkillCard() override = default;
};

/**
 * Power card class
 * TODO: Add member variables and functions as needed
 * You can modify the destructor if needed
 */
class PowerCard: public Card {
private:
    int plus_count_;
public:
    explicit PowerCard(int plus_count);
    void play(GameState &game_state) override;
    ~PowerCard() override = default;
};

/** 
 * Linked list node structure
 * TODO: Add or modify member variables and functions as needed
 */
struct Node {
    Card* card;
    Node* next;
    Node* prev;
    Node(Card* c) : card(c), next(nullptr), prev(nullptr) {}
};

/** 
 * Pile class
 * All three piles (draw pile, discard pile, hand pile) are represented using this class
 * TODO: Modify as needed, add member variables and functions
 */
class Pile {
private:
    int size_;
    Node* head_;
    Node* tail_;

public:
    friend void outShuffle(GameState&);
    friend void inShuffle(GameState&);
    friend void oddEvenShuffle(GameState&);
    friend class GameController;

    // Constructor and destructor declarations
    // Please implement them in the later part of the file
    Pile();
    ~Pile();

    // Return pile size
    int size() const {
        return size_;
    }

    // Check if pile is empty
    bool empty() const {
        return size_ == 0;
    }

    // Add a card to the end of the pile
    // This function is used to initialize the pile in main.cpp
    // Please implement this function in the later part of the file
    void appendCard(Card* card);
    
    // Helper methods
    void pushBack(Node* node);
    void pushFront(Node* node);
    Node* popFront();
    Node* popBack();
};

/**
 * Game state structure
 * Stores current game state information such as discard pile, draw pile, hand pile status, total score, current multiplier, etc.
 * TODO: Add member variables and functions as needed
 */
struct GameState {
    Pile hand_{};            // Hand pile
    Pile draw_pile_{};       // Draw pile
    Pile discard_pile_{};    // Discard pile
    long long total_score_ = 0;
    int current_multiplier_ = 1;
    int cumulative_power_ = 0;
};

/** 
 * Shuffle function declarations
 * outShuffle: Shuffle the discard pile into the draw pile in order
 * inShuffle: Shuffle the discard pile into the draw pile in reverse order
 * oddEvenShuffle: Separate the discard pile by odd and even positions, with odd positions first and even positions last
 * Please implement these functions in the later part of this file
*/
void outShuffle(GameState &game_state);
void inShuffle(GameState &game_state);
void oddEvenShuffle(GameState &game_state);

/**
 * Game controller class
 * Used to handle game logic
 */
class GameController {
private:
    GameState game_state_;
    // Shuffle function pointer
    void (*shuffle_)(GameState&);
public:
    explicit GameController(int mode);

    // Draw 5 cards from the draw pile to the hand pile
    // This is a function declaration, please implement this function in the later part of the file
    void draw();

    // Play the card_to_play-th card in the hand pile (counting from 1)
    // This is a function declaration, please implement this function in the later part of the file
    void play(int card_to_play);

    // End the current turn, move all cards in the hand pile to the discard pile
    // This is a function declaration, please implement this function in the later part of the file
    void finish();

    // Shuffle in the specified manner
    // This is a function declaration, please implement this function in the later part of the file
    void shuffle();

    // Query current total score
    // This is a function declaration, please implement this function in the later part of the file
    int queryScore();

    // Query draw pile size
    int queryDrawPileSize() const {
        return game_state_.draw_pile_.size();
    }

    // Query hand pile size
    int queryHandSize() const {
        return game_state_.hand_.size();
    }

    // Query discard pile size
    int queryDiscardPileSize() const {
        return game_state_.discard_pile_.size();
    }

    // Get reference to the draw pile
    // Used to load cards in main.cpp
    Pile& drawPile() {
        return game_state_.draw_pile_;
    }
};

// ================= 洗牌函数实现 ===================

/**
 * Shuffle the discard pile into the draw pile in order
 * TODO: Implement this function
 */
void outShuffle(GameState& game_state) {
    while (!game_state.discard_pile_.empty()) {
        game_state.draw_pile_.pushBack(game_state.discard_pile_.popFront());
    }
}

/**
 * Shuffle the discard pile into the draw pile in reverse order
 * TODO: Implement this function
 */
void inShuffle(GameState& game_state) {
    while (!game_state.discard_pile_.empty()) {
        game_state.draw_pile_.pushBack(game_state.discard_pile_.popBack());
    }
}

/**
 * Separate the discard pile by odd and even positions, with odd positions first and even positions last, and shuffle into the draw pile
 * TODO: Implement this function
 */
void oddEvenShuffle(GameState& game_state) {
    Pile odd_pile, even_pile;
    int i = 1;
    while (!game_state.discard_pile_.empty()) {
        Node* node = game_state.discard_pile_.popFront();
        if (i % 2 != 0) {
            odd_pile.pushBack(node);
        } else {
            even_pile.pushBack(node);
        }
        i++;
    }
    while (!odd_pile.empty()) {
        game_state.draw_pile_.pushBack(odd_pile.popFront());
    }
    while (!even_pile.empty()) {
        game_state.draw_pile_.pushBack(even_pile.popFront());
    }
}
    
// ======================================================




// ================== Game State Structure Implementation ===================
// If you added functions to the GameState structure, you can implement them here





// ======================================================




// ================= Card Class Implementation ===========================

// === Score Card Class Implementation ===
ScoreCard::ScoreCard(int point) : point_(point) {
}

void ScoreCard::play(GameState &game_state) {
    game_state.total_score_ += (long long)(point_ + game_state.cumulative_power_) * game_state.current_multiplier_;
    game_state.current_multiplier_ = 1;
}
// ===================

// === Skill Card Class Implementation ===
SkillCard::SkillCard(SkillCardType skill_card_type) : type_(skill_card_type) {
}
void SkillCard::play(GameState &game_state) {
    if (type_ == MULTIPLIER) {
        game_state.current_multiplier_ += 1;
    } else if (type_ == HEAD_BUTT) {
        if (!game_state.discard_pile_.empty()) {
            game_state.draw_pile_.pushFront(game_state.discard_pile_.popBack());
        }
    }
}
// ===================

// === Power Card Class Implementation ===
PowerCard::PowerCard(int plus_count) : plus_count_(plus_count) {
}
void PowerCard::play(GameState &game_state) {
    game_state.cumulative_power_ += plus_count_;
}
// ===================

// ======================================================




// ================= Pile Class Implementation ===========================
Pile::Pile() : size_(0), head_(nullptr), tail_(nullptr) {
}
Pile::~Pile() {
    while (head_) {
        Node* temp = head_;
        head_ = head_->next;
        delete temp->card;
        delete temp;
    }
}

/**
  * Add a card to the end of the pile
  * TODO: Implement this function
  */
void Pile::appendCard(Card* card) {
    pushBack(new Node(card));
}

void Pile::pushBack(Node* node) {
    node->next = nullptr;
    node->prev = tail_;
    if (tail_) {
        tail_->next = node;
    } else {
        head_ = node;
    }
    tail_ = node;
    size_++;
}

void Pile::pushFront(Node* node) {
    node->prev = nullptr;
    node->next = head_;
    if (head_) {
        head_->prev = node;
    } else {
        tail_ = node;
    }
    head_ = node;
    size_++;
}

Node* Pile::popFront() {
    if (!head_) return nullptr;
    Node* node = head_;
    head_ = head_->next;
    if (head_) {
        head_->prev = nullptr;
    } else {
        tail_ = nullptr;
    }
    node->next = node->prev = nullptr;
    size_--;
    return node;
}

Node* Pile::popBack() {
    if (!tail_) return nullptr;
    Node* node = tail_;
    tail_ = tail_->prev;
    if (tail_) {
        tail_->next = nullptr;
    } else {
        head_ = nullptr;
    }
    node->next = node->prev = nullptr;
    size_--;
    return node;
}

// ======================================================




// ================= Game Controller Class Implementation ======================

/**
 * Constructor, initialize game state and shuffle mode
 * @param mode Shuffle mode, 1 for outShuffle, 2 for inShuffle, 3 for oddEvenShuffle
 * TODO: Implement this function
 */
GameController::GameController(int mode) {
    if (mode == 1) shuffle_ = outShuffle;
    else if (mode == 2) shuffle_ = inShuffle;
    else if (mode == 3) shuffle_ = oddEvenShuffle;
    else shuffle_ = outShuffle;
}
/**
 * Draw 5 cards from the draw pile to the hand pile
 * TODO: Implement this function
 */
void GameController::draw() {
    for (int i = 0; i < 5; ++i) {
        if (game_state_.draw_pile_.empty()) {
            if (game_state_.discard_pile_.empty()) break;
            shuffle();
        }
        if (!game_state_.draw_pile_.empty()) {
            game_state_.hand_.pushBack(game_state_.draw_pile_.popFront());
        }
    }
}

/**
 * Play the card_to_play-th card in the hand pile (counting from 1)
 * TODO: Implement this function
 * Reference implementation approach:
    * 1. Find the corresponding hand card node
    * 2. Remove the card from the hand pile
    * 3. Execute the card's effect
    * 4. If it's a score card or skill card, move it to the discard pile; if it's a power card, delete it
 */
void GameController::play(int card_to_play) {
    if (card_to_play < 1 || card_to_play > game_state_.hand_.size()) return;
    Node* curr = game_state_.hand_.head_;
    for (int i = 1; i < card_to_play; ++i) {
        curr = curr->next;
    }
    
    // Remove from hand
    if (curr->prev) curr->prev->next = curr->next;
    else game_state_.hand_.head_ = curr->next;
    if (curr->next) curr->next->prev = curr->prev;
    else game_state_.hand_.tail_ = curr->prev;
    game_state_.hand_.size_--;
    
    Card* card = curr->card;
    card->play(game_state_);
    
    if (dynamic_cast<PowerCard*>(card)) {
        delete card;
        delete curr;
    } else {
        game_state_.discard_pile_.pushBack(curr);
    }
}

/**
 * Shuffle in the specified manner
 * TODO: Implement this function
 */
void GameController::shuffle() {
    shuffle_(game_state_);
}

/**
 * End the current turn, move all cards in the hand pile to the discard pile
 * TODO: Implement this function
 */
void GameController::finish() {
    while (!game_state_.hand_.empty()) {
        game_state_.discard_pile_.pushBack(game_state_.hand_.popFront());
    }
}

/**
 * Query current total score
 * TODO: Implement this function
 */
int GameController::queryScore() {
    return (int)game_state_.total_score_;
}

#endif //GAME_H
