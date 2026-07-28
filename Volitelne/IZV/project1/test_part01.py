#!/usr/bin/env python3
"""
Skript pro automaticke testovani prvni casti projektu.

Spousteni:
   pytest
nebo
   python3 -m pytest
"""
import part01
import numpy as np
import os
import pytest

def test_wave_original():
    """Test puvodni referencni implementace"""
    A = part01.wave_inference_bad(
        np.linspace(-10, 10, 50),
        np.linspace(-10, 10, 50),
        np.array([[-3, 0], [3, 0], [0, 4]]),
        2
    )



    np.testing.assert_allclose(A[25, 25], -0.23269281491671354)
    np.testing.assert_allclose(np.sum(A), -5.228034768146156)

def test_wave_your():
    """Test vase implementace"""
    A = part01.wave_inference_bad( 
        np.linspace(-10, 10, 50),
        np.linspace(-10, 10, 50),
        np.array([[-3, 0], [3, 0], [0, 4]]),
        2
    )
    # Provnani s vasi implementaci
    B = part01.wave_inference(
        np.linspace(-10, 10, 50),
        np.linspace(-10, 10, 50),
        np.array([[-3, 0], [3, 0], [0, 4]]),
        2
    )

    np.testing.assert_allclose(A, B)

def test_plot_wave():
    """Test generovani grafu s vlnami"""
    X = np.linspace(-10, 10, 200)
    Y = np.linspace(-10, 10, 200)
    A = part01.wave_inference( # muzete pouzit i vasi implementaci samozrejme
        X, Y,np.array([[-3, 0], [3, 0], [0, 4]]),
        2
    )
    part01.plot_wave(A, X, Y, show_figure=False, save_path="tmp_wave.png")
    assert os.path.exists("tmp_wave.png")


def test_generate_sin():
    """Test generovani grafu se sinusovkami"""
    part01.generate_sinus(show_figure=False, save_path="tmp_sin.png")
    assert os.path.exists("tmp_sin.png")


def test_download():
    """Test stazeni dat"""
    data = part01.download_data()

    assert len(data["positions"]) == 40
    assert len(data["lats"]) == 40
    assert len(data["longs"]) == 40
    assert len(data["heights"]) == 40

    assert data["positions"][0] == "Cheb"
    assert data["lats"][0] == pytest.approx(50.0683)
    assert data["longs"][0] == pytest.approx(12.3913)
    assert data["heights"][0] == pytest.approx(483.0)

