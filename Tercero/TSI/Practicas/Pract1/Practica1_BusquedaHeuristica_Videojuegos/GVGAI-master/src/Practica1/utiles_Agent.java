package Practica1;

import ontology.Types;
import tools.*;
import core.player.*;
import core.game.StateObservation;
import java.util.ArrayList;
import java.awt.Dimension;

public abstract class utiles_Agent extends AbstractPlayer {
	public utiles_Agent(StateObservation so, ElapsedCpuTimer elapsedTimer){}
	
	@Override
	public abstract Types.ACTIONS act(StateObservation stateObs, ElapsedCpuTimer elapsedTimer);
}

