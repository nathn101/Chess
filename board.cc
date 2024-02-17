#include "board.h"

using namespace std;

// TODO: // implement get all legal moves for a colour -- Replace logic in stalemate and checkmate
// TODO: make pawn not be able to go to last rank without promotion -- currrently legal

Board::Board(int size): Game{}, boardSize{size} {
    // Initialize theBoard
    SquareColour startColour;

    for (int i = 0; i < boardSize; i++) {
        // Set the starting colour
        if ( i % 2 == 0) startColour = SquareColour::White;
        else startColour = SquareColour::Burgundy;
        vector<Square> row;

        for (int j = 0; j < boardSize; j++) {
            // Add a new square to theBoard
            Square s{i, j, startColour};
            row.emplace_back(s);
            // Add squares to updated vector
            vector<int> square = {i, j};
            updated.emplace_back(square);
            // Change the colour for the next square
            startColour = (startColour == SquareColour::White) ? SquareColour::Burgundy : SquareColour::White;
        }
        // Insert the row
        theBoard.emplace_back(row);
    }

    // setup pieces on either side of the board
    int pawnRowWhite = boardSize - 2;
    int pawnRowBlack = 1;
    int pieceRowWhite = boardSize - 1;
    int pieceRowBlack = 0;

    // White side
    for (int i = 0; i < boardSize; i++) {
        this->addPiece(new Pawn{pawnRowWhite, i, Colour::White}, pawnRowWhite, i);
    }
    this->addPiece(new Rook{pieceRowWhite, 0, Colour::White}, pieceRowWhite, 0);
    this->addPiece(new Knight{pieceRowWhite, 1, Colour::White}, pieceRowWhite, 1);
    this->addPiece(new Bishop{pieceRowWhite, 2, Colour::White}, pieceRowWhite, 2);
    this->addPiece(new Queen{pieceRowWhite, 3, Colour::White}, pieceRowWhite, 3);
    this->addPiece(new King{pieceRowWhite, 4, Colour::White}, pieceRowWhite, 4);
    this->addPiece(new Bishop{pieceRowWhite, 5, Colour::White}, pieceRowWhite, 5);
    this->addPiece(new Knight{pieceRowWhite, 6, Colour::White}, pieceRowWhite, 6);
    this->addPiece(new Rook{pieceRowWhite, 7, Colour::White}, pieceRowWhite, 7);

    // black side
    for (int i = 0; i < boardSize; i++) {
        this->addPiece(new Pawn{pawnRowBlack, i, Colour::Black}, pawnRowBlack, i);
    }
    this->addPiece(new Rook{pieceRowBlack, 0, Colour::Black}, pieceRowBlack, 0);
    this->addPiece(new Knight{pieceRowBlack, 1, Colour::Black}, pieceRowBlack, 1);
    this->addPiece(new Bishop{pieceRowBlack, 2, Colour::Black}, pieceRowBlack, 2);
    this->addPiece(new Queen{pieceRowBlack, 3, Colour::Black}, pieceRowBlack, 3);
    this->addPiece(new King{pieceRowBlack, 4, Colour::Black}, pieceRowBlack, 4);
    this->addPiece(new Bishop{pieceRowBlack, 5, Colour::Black}, pieceRowBlack, 5);
    this->addPiece(new Knight{pieceRowBlack, 6, Colour::Black}, pieceRowBlack, 6);
    this->addPiece(new Rook{pieceRowBlack, 7, Colour::Black}, pieceRowBlack, 7);

    vector<vector<int>> update;

    // Initialize pseudo legal moves and attacking squares for the board
    for (auto& row : theBoard) {
        for (auto& cell : row) {
            update.emplace_back(vector<int>{cell.getRow(), cell.getCol()});
        }
    }

    updateAttacking(update, false);
}

Board::~Board() {
    for (auto row:theBoard) {
        for (auto cell: row) {
            delete cell.getPiece();
        }
    }
    // for (auto obs:observers) {
    //     delete obs;
    // }
}

float Board::getScore(PlayerColour c) {
    if (c == PlayerColour::White) {
        return whiteScore;
    } else if (c == PlayerColour::Black) {
        return blackScore;
    } else {
        return 0;
    }
}

void Board::clearBoard(){
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            removePiece(i, j);
        }
    }
}

void Board::attach(Observer *o) {
    observers.emplace_back(o);
}

bool Board::validLocation(string pos) {
    vector<int> loc = convertPos(pos);

    return (loc[0] <= boardSize - 1) && (loc[1] <= boardSize - 1) && (loc[0] >= 0) && (loc[1] >= 0);
}


bool Board::validLocation(int row, int col) {
    return (row <= boardSize - 1) && (col <= boardSize - 1) && (row >= 0) && (col >= 0);
}

Square& Board::getSquare(string location) {
    vector<int> locationVec = convertPos(location);
    return theBoard[locationVec[0]][locationVec[1]];
}

Square& Board::getSquare(int row, int col) {
    return theBoard[row][col];
}

void Board::addPiece(char piece, string location) {
    if (validLocation(location)) {
        Piece *p = convertPiece(piece, location);
        if (p != nullptr) {
            vector<int> locationVec = convertPos(location);
            theBoard[locationVec[0]][locationVec[1]].setPiece(p, true);
            updated.emplace_back(locationVec);
        } else {
            throw (std::invalid_argument("Error: PieceType not found"));
            return;
        }
    }
}

void Board::addPiece(Piece *p, int row, int col) {
    if (validLocation(row, col)) {
        theBoard[row][col].setPiece(p, true);
        vector<int> square = {row, col};
        updated.emplace_back(square);
    }
}

void Board::removePiece(string location) {
    if (validLocation(location)) {
        vector<int> locationVec = convertPos(location);
        theBoard[locationVec[0]][locationVec[1]].setPiece(new Empty{locationVec[0], locationVec[1]}, true);
        updated.emplace_back(locationVec);
    }
}

void Board::removePiece(int row, int col) {
    if (validLocation(row, col)) {
        theBoard[row][col].setPiece(new Empty{row, col}, true);
        vector<int> square = {row, col};
        updated.emplace_back(square);
    }
}

bool Board::isinTurn(PlayerColour c) const {
    if (getTurn() == c) {
        return true;
    }
    return false;
}

bool Board::setUpValid() {
    int whiteKing = 0;
    int blackKing = 0;

    // Check that the board contains exactly one white king and one black king
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            Piece *curr = theBoard[i][j].getPiece();
            if (curr != nullptr) {
                if (curr->getType() == PieceType::King && curr->getColour() == Colour::White) {
                    whiteKing++;
                } else if (curr->getType() == PieceType::King && curr->getColour() == Colour::Black) {
                    blackKing++;
                }
            }
        }
    }

    if (whiteKing != 1 || blackKing != 1) {
        return false;
    }

    // check that no pawns are on the first or last row of the board
    for (int i = 0; i < boardSize; i++) {
        Piece *firstRowPiece = theBoard[0][i].getPiece();
        Piece *lastRowPiece = theBoard[boardSize - 1][i].getPiece();
        if (firstRowPiece->getType() == PieceType::Pawn || lastRowPiece->getType() == PieceType::Pawn) {
            return false;
        }
    }

    vector<vector<int>> update;

    // Initialize pseudo legal moves and attacking squares for the board
    for (auto& row : theBoard) {
        for (auto& cell : row) {
            update.emplace_back(vector<int>{cell.getRow(), cell.getCol()});
        }
    }

    updateAttacking(update, false);

    // check if neither kings are in check
    if (isInCheck(PlayerColour::White) || isInCheck(PlayerColour::Black)) {
        return false;
    }

    return true; 
}

Piece* Board::convertPiece(char piece, string location) const {
    vector<int> locationVec = convertPos(location);
    Colour colour = (piece == 'K' || piece == 'Q' || piece == 'B' || piece == 'R' || piece == 'N' || piece == 'P') ? Colour::White : Colour::Black;

    // Convert the piece
    if (piece == 'K' || piece == 'k') {
        return new King{locationVec[0], locationVec[1], colour};
    } else if (piece == 'Q' || piece == 'q') {
        return new Queen{locationVec[0], locationVec[1], colour};
    } else if (piece == 'B' || piece == 'b') {
        return new Bishop{locationVec[0], locationVec[1], colour};
    } else if (piece == 'R' || piece == 'r') {
        return new Rook{locationVec[0], locationVec[1], colour};
    } else if (piece == 'N' || piece == 'n') {
        return new Knight{locationVec[0], locationVec[1], colour};
    } else if (piece == 'P' || piece == 'p'){
        return new Pawn{locationVec[0], locationVec[1], colour};
    } else {
        return nullptr;
    }
}

// TODO - Edit to take in capital letters like A8
vector<int> Board::convertPos(string pos) const {
    int row = '8' - pos[1];
    int col = pos[0] - 'a';
    return {row, col};
}

string Board::convertPos(vector<int> pos) const {
    char letter = pos[1] + 'a';
    char number = 8 - pos[0] + '0';

    string position = "";

    position += letter;
    position += number;

    return position;
}

void Board::notifyObservers() {
    for (auto ob:observers) ob->notify(*this);
}

void Board::endGame(PlayerColour winner) {
    if ( (winner == PlayerColour::White) || (winner == PlayerColour::Black) ) {
        addToScore(winner, 1);
    } else {
        addToScore(winner, 0.5);
    }
}

// TODO -- Out of bounds checks
bool Board::move(string from, string to) {
    // Convert string positions to vectors
    vector<int> fromVec = convertPos(from);
    vector<int> toVec = convertPos(to);

    if (!isLegalMove(fromVec, toVec)) return false;

    SpecialMoveType specialMoveType;
    bool specialMove = false;

    // get the piece on the current square
    Piece* moveP = getSquare(from).getPiece();

    // Get piece Colour
    Colour pieceColour = moveP->getColour();

	// Out of turn check 
    if ( (pieceColour == Colour::White && getTurn() == PlayerColour::Black) ||
         (pieceColour == Colour::Black && getTurn() == PlayerColour::White) ) {
        return false;
    }

    // Don't let pawns reach the last rank
    if ( (moveP->getType() == PieceType::Pawn) && (pieceColour == Colour::White) && (toVec[0] == 0)) return false;
    if ( (moveP->getType() == PieceType::Pawn) && (pieceColour == Colour::Black) && (toVec[0] == boardSize - 1)) return false;


    if (pieceColour == Colour::White) {
        // White short castle
        if ( (moveP->getType() == PieceType::King) && (fromVec[0] == boardSize - 1) && 
            (fromVec[1] == 4) && (toVec[0] == boardSize - 1) && (toVec[1] == 6) ) {
                specialMoveType = SpecialMoveType::ShortCastleW;
                specialMove = true;
        }

        // White long castle
        if ( (moveP->getType() == PieceType::King) && (fromVec[0] == boardSize - 1) && 
            (fromVec[1] == 4) && (toVec[0] == boardSize - 1) && (toVec[1] == 2) ) {
                specialMoveType = SpecialMoveType::LongCastleW;
                specialMove = true;
        }
    } else if (pieceColour == Colour::Black) {
        // Black short castle
        if ( (moveP->getType() == PieceType::King) && (fromVec[0] == 0) && 
            (fromVec[1] == 4) && (toVec[0] == 0) && (toVec[1] == 6) ) {
                specialMoveType = SpecialMoveType::ShortCastleB;
                specialMove = true;
        }

        // Black long castle
        if ( (moveP->getType() == PieceType::King) && (fromVec[0] == 0) && 
            (fromVec[1] == 4) && (toVec[0] == 0) && (toVec[1] == 2) ) {
                specialMoveType = SpecialMoveType::LongCastleB;
                specialMove = true;
        }
    }

    // En passant
    if ( (moveP->getType() == PieceType::Pawn) && ( (absVal(fromVec[0] - toVec[0]) == 1) && (absVal(fromVec[1] - toVec[1]) == 1) ) && canEnPassant(from, to) ) {
            specialMoveType = SpecialMoveType::EnPassant;
            specialMove = true;
    }

    vector<vector<int>> recentlyMoved;
    if (specialMove) {
        Piece *pieceMoved2;
        switch (specialMoveType) {
            case SpecialMoveType::ShortCastleW:
                pieceMoved2 = getSquare(boardSize - 1, 7).getPiece(); // Rook
                // Move king
                getSquare(fromVec[0], fromVec[1]).setPiece(new Empty{fromVec[0], fromVec[1]}, false);
                getSquare(toVec[0], toVec[1]).setPiece(moveP, true);
                // Move rook
                getSquare(boardSize - 1, 7).setPiece(new Empty{boardSize - 1, 7}, false);
                getSquare(boardSize - 1, 5).setPiece(pieceMoved2, true);

                // Update moved flag and position
                pieceMoved2->setMoved();
                moveP->setMoved();
                moveP->setPos(toVec[0], toVec[1]);
                pieceMoved2->setPos(boardSize - 1, 5);

                // Update the squares that the piece passed
                moveP->updatePassed(vector<vector<int>>{vector<int>{boardSize - 1, 5}});
                pieceMoved2->updatePassed(vector<vector<int>>{vector<int>{boardSize - 1, 6}});

                // Update recently moved squares
                recentlyMoved.emplace_back(moveP->getPos());
                recentlyMoved.emplace_back(pieceMoved2->getPos());

                updated.emplace_back(vector<int>{boardSize - 1, 7});
                updated.emplace_back(vector<int>{boardSize - 1, 5});

                break;
            case SpecialMoveType::ShortCastleB:
                pieceMoved2 = getSquare(0, 7).getPiece();  
                // Move king
                getSquare(fromVec[0], fromVec[1]).setPiece(new Empty{fromVec[0], fromVec[1]}, false);
                getSquare(toVec[0], toVec[1]).setPiece(moveP, true);
                // Move rook
                getSquare(0, 7).setPiece(new Empty{0, 7}, false);
                getSquare(0, 5).setPiece(pieceMoved2, true);

                pieceMoved2->setMoved();
                moveP->setMoved();
                moveP->setPos(toVec[0], toVec[1]);
                pieceMoved2->setPos(0, 5);

                moveP->updatePassed(vector<vector<int>>{vector<int>{0, 5}});
                pieceMoved2->updatePassed(vector<vector<int>>{vector<int>{0, 6}});

                recentlyMoved.emplace_back(moveP->getPos());
                recentlyMoved.emplace_back(pieceMoved2->getPos());

                updated.emplace_back(vector<int>{0, 7});
                updated.emplace_back(vector<int>{0, 5});

                break;
            case SpecialMoveType::LongCastleW:
                pieceMoved2 = getSquare(boardSize - 1, 0).getPiece();
                // Move king
                getSquare(fromVec[0], fromVec[1]).setPiece(new Empty{fromVec[0], fromVec[1]}, false);
                getSquare(toVec[0], toVec[1]).setPiece(moveP, true);
                // Move rook
                getSquare(boardSize - 1, 0).setPiece(new Empty{boardSize - 1, 0}, false);
                getSquare(boardSize - 1, 3).setPiece(pieceMoved2, true);

                pieceMoved2->setMoved();
                moveP->setMoved();
                moveP->setPos(toVec[0], toVec[1]);
                pieceMoved2->setPos(boardSize - 1, 3);

                moveP->updatePassed(vector<vector<int>>{vector<int>{boardSize - 1, 3}});
                pieceMoved2->updatePassed(vector<vector<int>>{vector<int>{boardSize - 1, 1}, vector<int>{boardSize - 1, 2}});

                recentlyMoved.emplace_back(moveP->getPos());
                recentlyMoved.emplace_back(pieceMoved2->getPos());

                updated.emplace_back(vector<int>{boardSize - 1, 0});
                updated.emplace_back(vector<int>{boardSize - 1, 3});
                break;
            case SpecialMoveType::LongCastleB:
                pieceMoved2 = getSquare(0, 0).getPiece();
                // Move king
                getSquare(fromVec[0], fromVec[1]).setPiece(new Empty{fromVec[0], fromVec[1]}, false);
                getSquare(toVec[0], toVec[1]).setPiece(moveP, true);
                // Move rook
                getSquare(0, 0).setPiece(new Empty{0, 0}, false);
                getSquare(0, 3).setPiece(pieceMoved2, true);

                pieceMoved2->setMoved();
                moveP->setMoved();
                moveP->setPos(toVec[0], toVec[1]);
                pieceMoved2->setPos(0, 3);

                moveP->updatePassed(vector<vector<int>>{vector<int>{0, 3}});
                pieceMoved2->updatePassed(vector<vector<int>>{vector<int>{0, 1}, vector<int>{0, 2}});

                recentlyMoved.emplace_back(moveP->getPos());
                recentlyMoved.emplace_back(pieceMoved2->getPos());

                updated.emplace_back(vector<int>{0, 0});
                updated.emplace_back(vector<int>{0, 3});

                break;
            case SpecialMoveType::EnPassant:
                Piece *pieceAffected = (moveP->getColour() == Colour::White) ? getSquare(toVec[0] + 1, toVec[1]).getPiece() :
                                                                                getSquare(toVec[0] - 1, toVec[1]).getPiece();

                vector<int> capturedPos = pieceAffected->getPos();

                // Edit the board
                getSquare(fromVec[0], fromVec[1]).setPiece(new Empty{fromVec[0], fromVec[1]}, false);
                getSquare(toVec[0], toVec[1]).setPiece(moveP, true);
                getSquare(capturedPos[0], capturedPos[1]).setPiece(new Empty{capturedPos[0], capturedPos[1]}, true);

                moveP->setMoved();
                moveP->setPos(toVec[0], toVec[1]);

                moveP->updatePassed(vector<vector<int>>{});

                recentlyMoved.emplace_back(moveP->getPos());

                updated.emplace_back(vector<int>{capturedPos[0], capturedPos[1]});
                break;
        }
    } else {
        // Edit the board
        getSquare(fromVec[0], fromVec[1]).setPiece(new Empty{fromVec[0], fromVec[1]}, false);
        getSquare(toVec[0], toVec[1]).setPiece(moveP, true);

        // Set the moved flag and position of the piece
        moveP->setMoved();
        moveP->setPos(toVec[0], toVec[1]);

        moveP->updatePassed(moveP->calcPath(fromVec, toVec));

        recentlyMoved.emplace_back(moveP->getPos());
    }

    updateAttacking(recentlyMoved, false);
    updated.emplace_back(fromVec); 
    updated.emplace_back(toVec);
    notifyObservers();

    return true;
}

bool Board::move(string from, string to, char promotion) {
    vector<int> toVec = convertPos(to);
    vector<int> fromVec = convertPos(from);

    if (!isLegalMove(fromVec, toVec)) return false;

    Piece* moveP = getSquare(from).getPiece();

    vector<vector<int>> recentlyMoved;
    // Get piece Colour
    Colour pieceColour = moveP->getColour();

    // Don't allow movement of empty squares
    if (pieceColour == Colour::NoColour) return false;

    // Out of turn check
    if ( (pieceColour == Colour::White && getTurn() == PlayerColour::Black) ||
         (pieceColour == Colour::Black && getTurn() == PlayerColour::White) ) {
        return false;
    }

    // See if the piece is a pawn and can move to the requested square
    if (moveP->getType() == PieceType::Pawn) {
        Piece* newP = convertPiece(promotion, to);
        // Move the Piece

        // Check that the promotion is valid
        if ( (newP == nullptr) || (newP->getColour() != moveP->getColour()) || 
        (newP->getType() == PieceType::Pawn) || (newP->getType() == PieceType::King) ) {
            return false;
        }
        
        // Edit the board
        getSquare(fromVec[0], fromVec[1]).setPiece(new Empty{fromVec[0], fromVec[1]}, false);
        getSquare(toVec[0], toVec[1]).setPiece(moveP, true);

        // Replace the old pawn with the new piece
        getSquare(toVec[0], toVec[1]).setPiece(newP, true);

        // Set the moved flag and position of the piece
        newP->setMoved();

        // Update the passed squares and add promotion square
        newP->updatePassed(vector<vector<int>>{});
        recentlyMoved.emplace_back(newP->getPos());

        updateAttacking(recentlyMoved, false);
        updated.emplace_back(fromVec); 
        updated.emplace_back(toVec);
        notifyObservers();

        return true;
    }
    return false;
}

bool Board::isLegalMove(vector<int> from, vector<int> to) {
    SpecialMoveType specialMoveType;
    bool enPassant = false;
    bool moveIsLegal = true;

    // Checks to see if any of the coordinates are out of bounds
    if ( !validLocation(from[0], from[1]) || !validLocation(to[0], to[1]) ) return false;

    // get the piece on the current square
    Piece* moveP = getSquare(from[0], from[1]).getPiece();
    Piece* destinationP = getSquare(to[0], to[1]).getPiece();

    // Get piece Colour
    Colour pieceColour = moveP->getColour();
    PlayerColour playerMoving = (pieceColour == Colour::White) ? PlayerColour::White : PlayerColour::Black;

    if (pieceColour == Colour::White) {
        // Note on castling: Due to the nature of castling, we already know if the player would be in check 
        // White short castle
        if ( (moveP->getType() == PieceType::King) && (from[0] == boardSize - 1) && 
            (from[1] == 4) && (to[0] == boardSize - 1) && (to[1] == 6) ) {
                specialMoveType = SpecialMoveType::ShortCastleW;
                return canCastle(specialMoveType);
        }

        // White long castle
        if ( (moveP->getType() == PieceType::King) && (from[0] == boardSize - 1) && 
            (from[1] == 4) && (to[0] == boardSize - 1) && (to[1] == 2) ) {
                specialMoveType = SpecialMoveType::LongCastleW;
                return canCastle(specialMoveType);
        }
    } else if (pieceColour == Colour::Black) {
        // Black short castle
        if ( (moveP->getType() == PieceType::King) && (from[0] == 0) && 
            (from[1] == 4) && (to[0] == 0) && (to[1] == 6) ) {
                specialMoveType = SpecialMoveType::ShortCastleB;
                return canCastle(specialMoveType);
        }

        // Black long castle
        if ( (moveP->getType() == PieceType::King) && (from[0] == 0) && 
            (from[1] == 4) && (to[0] == 0) && (to[1] == 2) ) {
                specialMoveType = SpecialMoveType::LongCastleB;
                return canCastle(specialMoveType);
        }
    }

    // En passant
    if ( (moveP->getType() == PieceType::Pawn) && ( (absVal(from[0] - to[0]) == 1) && (absVal(from[1] - to[1]) == 1) ) ) {
            specialMoveType = SpecialMoveType::EnPassant;
            enPassant = canEnPassant(convertPos(from), convertPos(to));
    }

    // Checks and prevents capturing current player's own piece
    if ( ( moveP->canMoveTo(from, to) || enPassant ) && 
       ( (destinationP->getColour() == Colour::White && pieceColour == Colour::Black) ||
         (destinationP->getColour() == Colour::Black && pieceColour == Colour::White) || 
         (destinationP->getColour() == Colour::NoColour) 
       ) ) {

        // Can't capture onto an empty piece if not en passent
        if ( (!enPassant) && (moveP->getType() == PieceType::Pawn) && ( (absVal(from[0] - to[0]) == 1) && (absVal(from[1] - to[1]) == 1) ) && (destinationP->getColour() == Colour::NoColour) ) return false;
        // Can't capture forward with a pawn
        if ( (moveP->getType() == PieceType::Pawn) && ( ( (absVal(from[0] - to[0]) == 1) || (absVal(from[0] - to[0]) == 2) ) && (absVal(from[1] - to[1]) == 0) ) && (destinationP->getColour() != Colour::NoColour) ) return false;

        // Get the piece to move and the piece to be affected (captured or empty)
        Piece *pieceMoved = getSquare(from[0], from[1]).getPiece();
        Piece *pieceAffected = getSquare(to[0], to[1]).getPiece();

        // Will hold the 'attacking' field of all of the pieces
        vector<vector<vector<int>>> stateAttacking;

        vector<vector<int>> recentlyMoved;

        // Saving data about the moved piece
        vector<vector<int>> movedPiecePassed;
        vector<vector<vector<int>>> movedPiecePseudoLegal;
        bool movedPieceMoved;
        vector<int> movedPiecePos;

        Piece *curr;

        // Save the attacking field of all pieces
        for (auto& row : theBoard) {
            for (auto& cell : row) {
                curr = cell.getPiece();
                stateAttacking.emplace_back(curr->getSquaresAttacking());
            }
        }

        vector<vector<int>> passed;

        // The captured piece is not on to if we have an en passant
        if (enPassant) {
            pieceAffected = (pieceMoved->getColour() == Colour::White) ? getSquare(to[0] + 1, to[1]).getPiece() :
                                                                            getSquare(to[0] - 1, to[1]).getPiece();
        }
        
        // Save the pseudo legal moves of the piece we moved
        movedPiecePseudoLegal = pieceMoved->getPseudoLegalMoves();

        // Save the passed squares of the piece we moved
        movedPiecePassed = pieceMoved->getPassed();

        // Update passed, and see if the move is not legal
        if (enPassant) {
            pieceMoved->updatePassed(vector<vector<int>>{});
        } else {
            // Calcuates the squares that have been passed so far
            passed = moveP->calcPath(from, to);

            // Checks if any of the squares passed contain a piece if so then don't execute the move
            for (size_t i = (size_t)0; i < passed.size(); ++i) {
                if (getSquare(passed[i][0], passed[i][1]).getPiece()->getType() != PieceType::Empty) moveIsLegal = false;
            }

            pieceMoved->updatePassed(passed);
        }

        // Edit the board position

        // Set the starting square to empty and the new square to the new piece
        getSquare(from[0], from[1]).setPiece(new Empty{from[0], from[1]}, false);
        
        if (enPassant) {
            getSquare(pieceAffected->getPos()[0], pieceAffected->getPos()[1]).setPiece(new Empty{pieceAffected->getPos()[0], pieceAffected->getPos()[1]}, false);
            getSquare(to[0], to[1]).setPiece(pieceMoved, true);
        } else {
            getSquare(to[0], to[1]).setPiece(pieceMoved, false);
        }

        // Set variables related to the moved piece
        movedPieceMoved = pieceMoved->hasMoved();
        pieceMoved->setMoved();
        movedPiecePos = pieceMoved->getPos();
        pieceMoved->setPos(to[0], to[1]);

        // Update the attacking vectors of the board
        recentlyMoved.emplace_back(pieceMoved->getPos());
        updateAttacking(recentlyMoved, true);

        // If the move is legal and the player does not end up in check, then the move is absolutely legal
        if (moveIsLegal && !isInCheck(playerMoving)) {
            moveIsLegal = true;
        } else {
            moveIsLegal = false;
        }

        // If the move piece hadn't already moved, unset the moved flag
        if (!movedPieceMoved) pieceMoved->unsetMoved();
        // Reset the moved piece passed field
        pieceMoved->updatePassed(movedPiecePassed);
        // Reset the moved piece pseudo legal moved field
        pieceMoved->setPseudoLegalMoves(movedPiecePseudoLegal);
        // Reset the coordinates of the moved piece
        pieceMoved->setPos(movedPiecePos[0], movedPiecePos[1]);

        // Reset the state of the board
        getSquare(from[0], from[1]).setPiece(pieceMoved, true);

        if (enPassant) {
            getSquare(pieceAffected->getPos()[0], pieceAffected->getPos()[1]).setPiece(pieceAffected, true);
        }

        getSquare(to[0], to[1]).setPiece(destinationP, false);

        // Reset the squares that the pieces are attacking
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                curr = theBoard[i][j].getPiece();
                curr->updateAttacking(stateAttacking[(i * boardSize) + j]);
            }
        }

        // Restore the attacked squares of the board
        updateAttacking(vector<vector<int>>{from}, true);



        if (moveIsLegal) {
            return true;
        }

        return false;
    }

    return false; 
}

bool Board::canCastle(SpecialMoveType t) {
    Piece *king;
    Piece *rook;

    switch(t) {
        case SpecialMoveType::LongCastleB:
            king = getSquare(0, 4).getPiece();
            rook = getSquare(0, 0).getPiece();
            for (int i = 3; i >= 2; --i) {
                if (isAttacked(0, i, PlayerColour::White)) return false;
                if (getSquare(0, i).getPiece()->getType() != PieceType::Empty) return false;
            }
            if (isInCheck(PlayerColour::Black)) return false;
            break;
        case SpecialMoveType::LongCastleW:
            king = getSquare(boardSize - 1, 4).getPiece();
            rook = getSquare(boardSize - 1, 0).getPiece();
            for (int i = 3; i >= 2; --i) {
                if (isAttacked(boardSize - 1, i, PlayerColour::Black)) return false;
                if (getSquare(boardSize - 1, i).getPiece()->getType() != PieceType::Empty) return false;
            }
            if (isInCheck(PlayerColour::White)) return false;
            break;
        case SpecialMoveType::ShortCastleB:
            king = getSquare(0, 4).getPiece();
            rook = getSquare(0, 7).getPiece();
            for (int i = 5; i <= 6; ++i) {
                if (isAttacked(0, i, PlayerColour::White)) return false;
                if (getSquare(0, i).getPiece()->getType() != PieceType::Empty) return false;
            }
            if (isInCheck(PlayerColour::Black)) return false;
            break;
        case SpecialMoveType::ShortCastleW:
            king = getSquare(boardSize - 1, 4).getPiece();
            rook = getSquare(boardSize - 1, 7).getPiece();
            for (int i = 5; i <= 6; ++i) {
                if (isAttacked(boardSize - 1, i, PlayerColour::Black)) return false;
                if (getSquare(boardSize - 1, i).getPiece()->getType() != PieceType::Empty) return false;
            }
            if (isInCheck(PlayerColour::White)) return false;
            break;
        default:
            return false;
    }

    if ( (king->getType() != PieceType::King) || (rook->getType() != PieceType::Rook) ) return false;
    if (king->hasMoved() || rook->hasMoved()) return false;

    return true;
}

bool Board::canEnPassant(string from, string to) {
    vector<int> fromVec = convertPos(from);
    vector<int> toVec = convertPos(to);

    Piece *passenter = getSquare(fromVec[0], fromVec[1]).getPiece();
    Piece *passented = (passenter->getColour() == Colour::White) ? getSquare(toVec[0] + 1, toVec[1]).getPiece() :
                                                                   getSquare(toVec[0] - 1, toVec[1]).getPiece();

    // Make sure the pieces are correct
    if (passenter->getType() != PieceType::Pawn ||
        passented->getType() != PieceType::Pawn) {
        return false;
    }

    // Make sure the passented has just moved
    if (Pawn* pawnPtr = dynamic_cast<Pawn *>(passented)) {
        if (!pawnPtr->hasJustMoved()) return false;
    } else {
        throw (std::bad_cast());
        return false;
    }

    // Make sure the pieces are not the same colour
    if (passenter->getColour() == passented->getColour()) {
        return false;
    }

    // Make sure the pawn is moving from the right square
    if (absVal(fromVec[0] - toVec[0]) != 1 || absVal(fromVec[1] - toVec[1]) != 1) {
        return false;
    }

    // Make sure the pawn moved 2 squares on the last turn
    if (passented->getPassed() != vector<vector<int>>{toVec}) {
        return false;
    }

    return true;

    // TODO -- Edit this function to actually enforce that the first pawn move needs to have been made LAST TURN -- Not that it was just the most recent move by the pawn
}



bool Board::isInCheck(PlayerColour c) {
    // Find the king of the specified color
    Colour pieceColour;
    (c == PlayerColour::White)? pieceColour = Colour::White : pieceColour = Colour::Black;
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            Piece* cur = getSquare(i, j).getPiece();
            if (cur->getType() == PieceType::King && cur->getColour() == pieceColour) {
                return (c == PlayerColour::White) ? isAttacked(i, j, PlayerColour::Black) : isAttacked(i, j, PlayerColour::White);
            }
        }
    }
    // Surpress warnings from compiler
    return false;
}

bool Board::isCheckmated(PlayerColour c) {
    // Check if the king is in check
    if (!isInCheck(c)) {
        return false;  // King is not in check, not checkmate
    }

    return getLegalMoves(c).size() == 0;
}
        
bool Board::isStalemated(PlayerColour c) {
    // Check if the king is not in check
    if (isInCheck(c)) {
         return false;  // King is in check, not stalemate
    }

    return getLegalMoves(c).size() == 0;
}

// TODO -- Changes in Piece, and also know what squares have been updated (moved piece, along with any captures)
void Board::updateAttacking(vector<vector<int>> recentlyMoved, bool simulatedMove) {
    blackAttacking.clear();
    whiteAttacking.clear();

    Piece *curr;
    PieceType t;
    Colour c;
    bool recently;

    // Update all sliding pieces and recently moved pieces
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            curr = theBoard[i][j].getPiece();
            t = curr->getType();
            c = curr->getColour();

            // Attempt to dynamic_cast to Pawn
            if (t == PieceType::Pawn && !simulatedMove) {
                // Update the Pawn's justMoved field (en-passant purposes)
                if (Pawn* pawnPtr = dynamic_cast<Pawn *>(curr)) {
                    pawnPtr->updateJustMoved();
                } else {
                    throw (std::bad_cast());
                }
            }

            // Check if we are on a square that has recently been changed
            for (auto v: recentlyMoved) {
                if (v[0] == i && v[1] == j) {
                    recently = true;
                    break;
                } else {
                    recently = false;
                }
            }

            if ( recently ) {
                updateAttackingAt(i, j, true);
            } else if ( (t == PieceType::Rook) || (t == PieceType::Bishop) || (t == PieceType::Queen) ) {
                updateAttackingAt(i, j, false);
            }

            auto attacking = curr->getSquaresAttacking();

            if (c == Colour::White) {
                for (size_t i = (size_t)0; i < attacking.size(); ++i) {
                    whiteAttacking.emplace_back(attacking[i]);
                }
            } else {
                for (size_t i = (size_t)0; i < attacking.size(); ++i) {
                    blackAttacking.emplace_back(attacking[i]);
                }
            }
        }
    }
}

void Board::updateAttackingAt(int row, int col, bool updatePseudoLegal) {
    // Get pseudo legal squares, iterate and cut off
    Piece *p = theBoard[row][col].getPiece();

    if (updatePseudoLegal) {
        p->updatePseudoLegalMoves(boardSize);
    }
    vector<vector<vector<int>>> pseudoLegal = p->getPseudoLegalMoves();
    vector<vector<int>> attacking;
    
    for (size_t i = (size_t)0; i < pseudoLegal.size(); ++i) {
        for (size_t j = (size_t)0; j < pseudoLegal[i].size(); ++j) {
            // We don't consider pawns to be attacking squares in front of them
            if ((p->getType() == PieceType::Pawn) && ( (absVal(pseudoLegal[i][j][0] - p->getPos()[0]) == 1) ||
                                                       (absVal(pseudoLegal[i][j][0] - p->getPos()[0]) == 2) )) {
                pseudoLegal[i] = vector<vector<int>>{};
                break;
            }
            if (theBoard[pseudoLegal[i][j][0]][pseudoLegal[i][j][1]].getPiece()->getType() != PieceType::Empty) {
                // If it's a pawn and the pseudo legal involves a forward movement, disregard it
                pseudoLegal[i] = vector<vector<int>>(pseudoLegal[i].begin(), pseudoLegal[i].begin() + j + 1);
                break;
            }
        }
    }

    for (size_t i = (size_t)0; i < pseudoLegal.size(); ++i) {
        for (size_t j = (size_t)0; j < pseudoLegal[i].size(); ++j) {
            attacking.emplace_back(pseudoLegal[i][j]);
        }
    }

    p->updateAttacking(attacking);
}

bool Board::isAttacked(int row, int col, PlayerColour by) {
    if (by == PlayerColour::White) {
        for (const auto& attacking : whiteAttacking) {
            if (attacking[0] == row && attacking[1] == col) return true;
        }
        return false;
    } else if (by == PlayerColour::Black) {
        for (const auto& attacking : blackAttacking) {
            if (attacking[0] == row && attacking[1] == col) return true;
        }
        return false;
    } else {
        return false;
    }
}

vector<vector<vector<int>>> Board::getLegalMoves() {
    return getLegalMoves(getTurn());
}

vector<vector<vector<int>>> Board::getLegalMoves(PlayerColour c) {

    Colour pieceColour = (c == PlayerColour::White) ? Colour::White : Colour::Black;

    vector<vector<vector<int>>> moves;

    // Iterate over all pieces of the specified color and check if any valid move is available
    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            Piece* currentPiece = getSquare(row, col).getPiece();
            if (currentPiece->getColour() == pieceColour) {
                vector<int> from = currentPiece->getPos();
                // If the piece is a pawn, add its forward moves
                if (currentPiece->getType() == PieceType::Pawn) {
                    vector<vector<vector<int>>> pseudoLegal = currentPiece->getPseudoLegalMoves();

                    for (size_t i = (size_t)0; i < pseudoLegal.size(); ++i) {
                        for (size_t j = (size_t)0; j < pseudoLegal[i].size(); ++j) {
                            if (theBoard[pseudoLegal[i][j][0]][pseudoLegal[i][j][1]].getPiece()->getType() != PieceType::Empty) {
                                // If it's a pawn and the pseudo legal involves a forward movement, disregard it
                                pseudoLegal[i] = vector<vector<int>>(pseudoLegal[i].begin(), pseudoLegal[i].begin() + j + 1);
                                break;
                            }
                        }
                    }

                    for (size_t i = (size_t)0; i < pseudoLegal.size(); ++i) {
                        for (size_t j = (size_t)0; j < pseudoLegal[i].size(); ++j) {
                            if (isLegalMove(from, pseudoLegal[i][j])) {
                                moves.emplace_back(vector<vector<int>>{from, pseudoLegal[i][j]});
                            }
                        }
                    }
                }
                auto attacking = currentPiece->getSquaresAttacking();
                for (auto& to : attacking) {
                    if (isLegalMove(from, to)) {
                        moves.emplace_back(vector<vector<int>>{from, to});
                    }
                }
            }
        }
    }
    return moves;
}

Board::Board(Board& b) {
    this->boardSize = b.boardSize;
    // Ignore observers
    this->blackAttacking = b.blackAttacking;
    this->whiteAttacking = b.whiteAttacking;
    this->updated = b.updated;

    for (int i = 0; i < boardSize; i++) {
        vector<Square> row;

        for (int j = 0; j < boardSize; j++) {
            // Add a new square to theBoard
            Square s{i, j, b.theBoard[i][j].getColour()};
            Piece *otherPiece = b.theBoard[i][j].getPiece();
            Piece *p;
            switch (otherPiece->getType()) {
                case PieceType::Pawn:
                    if (Pawn* pawnPtr = dynamic_cast<Pawn *>(otherPiece)) {
                        p = new Pawn{pawnPtr->getPos()[0], pawnPtr->getPos()[1], pawnPtr->getColour(),
                                        pawnPtr->getType(), pawnPtr->getValue(), pawnPtr->getPassed(), 
                                        pawnPtr->getSquaresAttacking(), pawnPtr->hasMoved(), pawnPtr->hasJustMoved(), pawnPtr->getJustMovedBuffer()};
                        
                    } else {
                        p = new Pawn{otherPiece->getPos()[0], otherPiece->getPos()[1], otherPiece->getColour(),
                                        otherPiece->getType(), otherPiece->getValue(), otherPiece->getPassed(), 
                                        otherPiece->getSquaresAttacking(), otherPiece->hasMoved()};
                    }
                    break;
                case PieceType::Rook:
                    p = new Rook{otherPiece->getPos()[0], otherPiece->getPos()[1], otherPiece->getColour(),
                                    otherPiece->getType(), otherPiece->getValue(), otherPiece->getPassed(), 
                                    otherPiece->getSquaresAttacking(), otherPiece->hasMoved()};
                    break;
                case PieceType::Knight:
                    p = new Knight{otherPiece->getPos()[0], otherPiece->getPos()[1], otherPiece->getColour(),
                                    otherPiece->getType(), otherPiece->getValue(), otherPiece->getPassed(), 
                                    otherPiece->getSquaresAttacking(), otherPiece->hasMoved()};
                    break;
                case PieceType::Bishop:
                    p = new Bishop{otherPiece->getPos()[0], otherPiece->getPos()[1], otherPiece->getColour(),
                                    otherPiece->getType(), otherPiece->getValue(), otherPiece->getPassed(), 
                                    otherPiece->getSquaresAttacking(), otherPiece->hasMoved()};
                    break;
                case PieceType::Queen:
                    p = new Queen{otherPiece->getPos()[0], otherPiece->getPos()[1], otherPiece->getColour(),
                                    otherPiece->getType(), otherPiece->getValue(), otherPiece->getPassed(), 
                                    otherPiece->getSquaresAttacking(), otherPiece->hasMoved()};
                    break;
                case PieceType::King:
                    p = new King{otherPiece->getPos()[0], otherPiece->getPos()[1], otherPiece->getColour(),
                                    otherPiece->getType(), otherPiece->getValue(), otherPiece->getPassed(), 
                                    otherPiece->getSquaresAttacking(), otherPiece->hasMoved()};
                    break;
                case PieceType::Empty:
                    p = new Empty{otherPiece->getPos()[0], otherPiece->getPos()[1], otherPiece->getColour(),
                                    otherPiece->getType(), otherPiece->getValue(), otherPiece->getPassed(), 
                                    otherPiece->getSquaresAttacking(), otherPiece->hasMoved()};
                    break;
            }
            s.setPiece(p, true);
            row.emplace_back(s);
        }
        // Insert the row
        this->theBoard.emplace_back(row);
    }

    vector<vector<int>> update;

        // Initialize pseudo legal moves and attacking squares for the board
    for (auto& row : theBoard) {
        for (auto& cell : row) {
            update.emplace_back(vector<int>{cell.getRow(), cell.getCol()});
        }
    }

    updateAttacking(update, false);
}

// TODO -- Include delete statements when creating replica
bool Board::move() {
    PlayerColour pieceTurn;
    PlayerColour oppPieceTurn;
    Colour colourTurn;
    // Determining which colour has the current turn
    if (getTurn() == PlayerColour::White) {
        pieceTurn = PlayerColour::White;
        colourTurn = Colour::White;
        oppPieceTurn = PlayerColour::Black;
    } else {
        pieceTurn = PlayerColour::Black;
        colourTurn = Colour::Black;
        oppPieceTurn = PlayerColour::White;
    }
    // Try to cast to a cpu so that we can perform cpu methods on the player (eg. getLevel() which human does not have)
    cpu* computer = dynamic_cast<cpu *>(getPlayer(getTurn()));
    if (computer) {
        srand(time(0));
        int randomPos = rand();
        int randomPromotion = rand();
        int maxVal = INT_MIN;
        // int maxValIndex = 0;
        vector<char> whitePromotion = {'Q', 'R', 'B', 'K'};
        vector<char> blackPromotion = {'q', 'r', 'b', 'k'};
        vector<vector<vector<int>>> legalMoves = getLegalMoves();
        if (legalMoves.size() == 0) return false;
        vector<vector<int>> curMove;
        vector<vector<vector<int>>> preferredMoves;
        vector<vector<int>> bestMove;
        vector<int> kingPos;
        Board* replicaBoard;

        // locate the king
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                Piece *curr = theBoard[i][j].getPiece();
                if (curr != nullptr) {
                    if (curr->getType() == PieceType::King && curr->getColour() == Colour::Black && pieceTurn == PlayerColour::White) {
                        kingPos = curr->getPos();
                    } else if (curr->getType() == PieceType::King && curr->getColour() == Colour::White && pieceTurn == PlayerColour::Black) {
                        kingPos = curr->getPos();
                    }
                }
            }
        }

        switch(computer->getLevel()) {
            case 1: // lvl 1 cpu
                curMove = legalMoves.at(randomPos % legalMoves.size());
                // Checks if the piece being moved is a pawn
                if (getSquare(curMove[0][0], curMove[0][1]).getPiece()->getType() == PieceType::Pawn) {
                    // Checks if the destination is the back-rank
                    if (curMove[1][0] == 0 && pieceTurn == PlayerColour::White) {
                        // Chooses a random piece to promote to
                        move(convertPos(curMove[0]), convertPos(curMove[1]), whitePromotion[randomPromotion % 4]);
                    } else {
                        move(convertPos(curMove[0]), convertPos(curMove[1]));
                    }
                    // Checks if the destination is the back-rank
                    if (curMove[1][0] == 7 && pieceTurn == PlayerColour::Black) {
                        // Chooses a random piece to promote to
                        move(convertPos(curMove[0]), convertPos(curMove[1]), blackPromotion[randomPromotion % 4]);
                    } else {
                        move(convertPos(curMove[0]), convertPos(curMove[1]));
                    }
                } else {
                    move(convertPos(curMove[0]), convertPos(curMove[1]));
                }
                return true;
                break;
            case 2: // lvl 2 cpu
                for (size_t i = (size_t)0; i < legalMoves.size(); ++i) {
                    replicaBoard = new Board(*this);
                    curMove = legalMoves.at(i);
                    int val = getSquare(curMove[1][0], curMove[1][1]).getPiece()->getValue();
                    // preferring captures
                    if (val > 0) { 
                        preferredMoves.emplace_back(curMove);
                    } else {
                        // call on overloaded ctor that will return a replica of the board

                        replicaBoard->move(convertPos(vector<int>{curMove[0][0], curMove[0][1]}), convertPos(vector<int>{curMove[1][0], curMove[1][1]}));

                        if (replicaBoard->isAttacked(kingPos[0], kingPos[1], oppPieceTurn)) {
                            preferredMoves.emplace_back(curMove);
                        }
                    }

                    delete replicaBoard;
                }
                // Checks if there were any captures or checks even possible
                if (preferredMoves.size() > 0) {
                    bestMove = preferredMoves.at(randomPos % preferredMoves.size());
                } else { // Checks if there are no captures or checks available in which case it will just execute a random move
                    bestMove = legalMoves.at(randomPos % legalMoves.size());
                }

                // Checks if the piece being moved is a pawn
                if (getSquare(bestMove[0][0], bestMove[0][1]).getPiece()->getType() == PieceType::Pawn) {
                    // Checks if the destination is the back-rank
                    if (bestMove[1][0] == 0 && pieceTurn == PlayerColour::White) {
                        // Usually pawns promote near the endgame and the endgame favours long-range attacking pieces like queens, bishops, rooks
                        move(convertPos(bestMove[0]), convertPos(bestMove[1]), whitePromotion[randomPromotion % 3]);
                    } else {
                        move(convertPos(bestMove[0]), convertPos(bestMove[1]));
                    }
                    // Checks if the destination is the back-rank
                    if (bestMove[1][0] == 7 && pieceTurn == PlayerColour::Black) {
                        // Usually pawns promote near the endgame and the endgame favours long-range attacking pieces like queens, bishops, rooks
                        move(convertPos(bestMove[0]), convertPos(bestMove[1]), blackPromotion[randomPromotion % 3]);
                    } else {
                        move(convertPos(bestMove[0]), convertPos(bestMove[1]));
                    }
                } else {
                    move(convertPos(bestMove[0]), convertPos(bestMove[1]));
                }
                return true;
                break;
            case 3: // lvl 3 cpu
                for (size_t i = (size_t)0; i < legalMoves.size(); ++i) {
                  
                    replicaBoard = new Board(*this);

                    curMove = legalMoves.at(i);
                    // Gets value at "to" square
                    int val = getSquare(curMove[1][0], curMove[1][1]).getPiece()->getValue();
                    replicaBoard->move(convertPos(vector<int>{curMove[0][0], curMove[0][1]}), convertPos(vector<int>{curMove[1][0], curMove[1][1]}));

                    // preferring moving to squares that aren't getting attacked
                    if (!replicaBoard->isAttacked(curMove[1][0], curMove[1][1], oppPieceTurn)) {
                        preferredMoves.emplace_back(curMove);
                    }

                    // preferring captures
                    if (val >= maxVal) { 
                        preferredMoves.emplace_back(curMove);
                    } else {
                        // call on overloaded ctor that will return a replica of the board
                        
                        if (replicaBoard->isAttacked(kingPos[0], kingPos[1], pieceTurn)) {
                            // Checking if checking the opponents king will leave your piece under attack
                            if (!replicaBoard->isAttacked(curMove[1][0], curMove[1][1], oppPieceTurn)) {
                                preferredMoves.emplace_back(curMove);
                            }
                        }
                        // If current square is attacked then want to move away
                        if (replicaBoard->isAttacked(curMove[0][0], curMove[0][1], oppPieceTurn) && val > 400) {
                            preferredMoves.emplace_back(curMove);
                        }
                    }
                    // preferring moving to squares that aren't getting attacked
                    if (!replicaBoard->isAttacked(curMove[1][0], curMove[1][1], oppPieceTurn)) {
                        preferredMoves.emplace_back(curMove);
                    }
                }

                // Checks if there are any possible moves that avoid getting captured, capturing and checks
                if (preferredMoves.size() > 0) {
                    bestMove = preferredMoves.at(randomPos % preferredMoves.size());
                } else {
                    bestMove = legalMoves.at(randomPos % legalMoves.size());
                }
                // Checks if the piece being moved is a pawn
                if (getSquare(bestMove[0][0], bestMove[0][1]).getPiece()->getType() == PieceType::Pawn) {
                    // Checks if the destination is the back-rank
                    if (bestMove[1][0] == 0 && pieceTurn == PlayerColour::White) {
                        // Favours promotion to queen
                        move(convertPos(bestMove[0]), convertPos(bestMove[1]), whitePromotion[0]);
                    } else {
                        move(convertPos(bestMove[0]), convertPos(bestMove[1]));
                    }
                    // Checks if the destination is the back-rank
                    if (bestMove[1][0] == 7 && pieceTurn == PlayerColour::Black) {
                        // Favours promotion to queen
                        move(convertPos(bestMove[0]), convertPos(bestMove[1]), blackPromotion[0]);
                    } else {
                        move(convertPos(bestMove[0]), convertPos(bestMove[1]));
                    }
                } else {
                    move(convertPos(bestMove[0]), convertPos(bestMove[1]));
                }
                return true;
                break;
            case 4: // lvl 4 cpu
                for (size_t i = (size_t)0; i < legalMoves.size(); ++i) {
                    replicaBoard = new Board(*this);
                    curMove = legalMoves.at(i);
                    int val = getSquare(curMove[1][0], curMove[1][1]).getPiece()->getValue();
                    // Checks if the square is worth moving to or is a preferable square by using the PSTs
                    Square from = getSquare(curMove[0][0], curMove[0][1]);
                    Square to = getSquare(curMove[1][0], curMove[1][1]);
                    // call on overloaded ctor that will return a replica of the board
                        replicaBoard->move(convertPos(vector<int>{curMove[0][0], curMove[0][1]}), convertPos(vector<int>{curMove[1][0], curMove[1][1]}));
                    if (val + from.getPiece()->getPST(colourTurn)[to.getRow()][to.getCol()] >= maxVal) { 
                        // preferring moving to squares that aren't getting attacked
                        if (!replicaBoard->isAttacked(curMove[1][0], curMove[1][1], oppPieceTurn)) {
                            preferredMoves.emplace_back(curMove);
                            maxVal = val + from.getPiece()->getPST(colourTurn)[to.getRow()][to.getCol()];
                            // maxValIndex = i;
                        }
                        // Checking the king is more desireable than other moves
                        if (replicaBoard->isAttacked(kingPos[0], kingPos[1], pieceTurn)) {
                            if (!replicaBoard->isAttacked(curMove[1][0], curMove[1][1], oppPieceTurn)
                            || val + from.getPiece()->getPST(colourTurn)[to.getRow()][to.getCol()] + INT16_MAX >= maxVal) {
                                preferredMoves.emplace_back(curMove);
                                maxVal = val + from.getPiece()->getPST(colourTurn)[to.getRow()][to.getCol()] + INT16_MAX;
                                // maxValIndex = i;
                            }
                        }
                        // If current square is attacked then want to move away
                        if (replicaBoard->isAttacked(curMove[0][0], curMove[0][1], oppPieceTurn) && val > 400) {
                            preferredMoves.emplace_back(curMove);
                        }
                    }
                    delete replicaBoard;

                }
                // Choose from any one of the top 2 moves if there are more than 4 good moves
                if (preferredMoves.size() > 4) {
                    bestMove = preferredMoves[preferredMoves.size() - (randomPos % 2) - 1];
                } else {
                    if (preferredMoves.size() == 0) {
                        bestMove = legalMoves[randomPos % legalMoves.size()];
                    } else {
                        bestMove = preferredMoves[preferredMoves.size() - 1];
                    }
                }
                // Checks if the piece being moved is a pawn
                if (getSquare(bestMove[0][0], bestMove[0][1]).getPiece()->getType() == PieceType::Pawn) {
                    // Checks if the destination is the back-rank
                    if (bestMove[1][0] == 0 && pieceTurn == PlayerColour::White) {
                        // Favours promotion to queen
                        move(convertPos(bestMove[0]), convertPos(bestMove[1]), whitePromotion[0]);
                    } else {
                        move(convertPos(bestMove[0]), convertPos(bestMove[1]));
                    }
                    // Checks if the destination is the back-rank
                    if (bestMove[1][0] == 7 && pieceTurn == PlayerColour::Black) {
                        // Favours promotion to queen
                        move(convertPos(bestMove[0]), convertPos(bestMove[1]), blackPromotion[0]);
                    } else {
                        move(convertPos(bestMove[0]), convertPos(bestMove[1]));
                    }
                } else {
                    move(convertPos(bestMove[0]), convertPos(bestMove[1]));
                }
                return true;
                break;
            default:
                return false;
                break;
        }
    } else {
        return false;
    }
    return false;
}

vector<vector<int>>& Board::getUpdated() {
    return updated;
}

void Board::clearUpdated() {
    for (auto& outerVec : updated) {
        outerVec.clear();
    }
    updated.clear();
}

// TODO -- 50 move rule, 3fold repitition, insufficient material (not required)
bool Board::nextTurn() {
    PlayerColour next = (getTurn() == PlayerColour::Black) ? PlayerColour::White : PlayerColour::Black;
    if (isStalemated(next)) {
        cout << "Stalemate!" << endl;
        endGame(PlayerColour::NoColour);
        return false;
    } else if (isCheckmated(next)) {
        cout << ((next == PlayerColour::White) ? "White is checkmated!" : "Black is checkmated!") << endl;
        endGame(getTurn());
        return false;
    } else if (isInCheck(next)) {
        cout << ((next == PlayerColour::White) ? "White is in check!" : "Black is in check!") << endl;
        setTurn(next);
        return true;
    }
    setTurn(next);
    return true;
}

void Board::printEvaluation() {
    int blackPieceValue = 0;
    int whitePieceValue = 0;
    for (auto &row : theBoard) {
        for (auto& cell: row) {
            Piece* p = cell.getPiece();
            // Add the value of the pieces
            if (p->getType() != PieceType::King) {
                (p->getColour() == Colour::White) ? whitePieceValue += (p->getValue() / 100) : blackPieceValue += (p->getValue() / 100);
            }
        }
    }

    int evaluation = whitePieceValue - blackPieceValue;

    cout << "Evaluation: " << (evaluation > 0 ? "+" :"") << whitePieceValue - blackPieceValue << endl;
}
