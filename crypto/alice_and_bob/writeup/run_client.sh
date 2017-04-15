#!/bin/bash
trust anchor --store cert.pem
../src/client/client
trust anchor --remove cert.pem
