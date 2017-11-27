WorldInitializer::WorldInitializer() {
}

void WorldInitializer::parse_inputs() {
    cin >> this->my_score;
    
    cin >> this->enemy_score_1;
    
    cin >> this->enemy_score_2;
    
    cin >> this->my_rage;
    
    cin >> this->enemy_rage_1;

    cin >> this->enemy_rage_2;
    
    int unit_count;
    cin >> unit_count;

    #if PRINT_INPUTS
        cerr << this->my_score << endl;
        cerr << this->enemy_score_1 << endl;
        cerr << this->enemy_score_2 << endl;
        cerr << this->my_rage << endl;
        cerr << this->enemy_rage_1 << endl;
        cerr << this->enemy_rage_2 << endl;
        cerr << unit_count << endl;
    #endif

    for (int i = 0; i < unit_count; i++) {
        UnitInitializer u;
        cin >> u.unit_id >> u.unit_type >> u.player >> u.mass >> u.radius >> u.x >> u.y >> u.vx >> u.vy >> u.extra >> u.extra2;
        this->units_initializer.push_back(u);

        #if PRINT_INPUTS
            cerr << u.unit_id << " " << u.unit_type << " " << u.player << " " << u.mass << " ";
            cerr << u.radius << " " << u.x << " " << u.y << " " << u.vx << " ";
            cerr << u.vy << " " << u.extra << " " << u.extra2 << endl;
        #endif
    }
}