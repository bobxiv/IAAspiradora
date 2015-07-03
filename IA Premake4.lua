--Archivo de lua como programa para premake4
--Genera las soluciones y Proyectos/make file 
--
--Ejecutar con: premake4.exe --file="IA Premake4.lua" |vs2010
--													  |vs2008
--													  |gmake
--													  |etc
--
--Para mas informacion correr: premake4.exe --help

solution("IA")
	configurations({"Debug","Release"})
	
	configuration("Debug")
		defines("_DEBUG")
		flags({"Symbols"})
	
	configuration("Release")	
		flags({"OptimizeSpeed"})
	
	--Proyecto de Libreria Principal
	project("IA Core")
	kind("StaticLib")
	language("C++")
	location("IA Core")
	files({	"IA Core/Headers/*.h", --Carpeta de encabezados
			"IA Core/Source/*.cpp" --Carpeta de fuentes
		  })
	includedirs("IA Core/Headers/")--Carpeta de Includes
	
	configuration("Debug")
		targetdir("IA Core/Bin/Debug")
	
	configuration("Release")
		targetdir("IA Core/Bin/Release")
	
	--Proyecto En particular
	project("Ejercicio Aspiradora")
	language("C++")
	location("Ejercicio Aspiradora")
	files({	"Ejercicio Aspiradora/Headers/*.h", --Carpeta de encabezados
			"Ejercicio Aspiradora/Source/*.cpp" --Carpeta de fuentes
		  })
		  
		 --IMPORTANTE EL ULTIMO PATH ES EL INCLUDE DE SFML 1.6 CAMBIARLO AL QUE VALLA
	includedirs({"Ejercicio Aspiradora/Headers/","IA Core/Headers/","D:/Libs/SFML/SFML-1.6/include/"})--Carpeta de Includes
	
	
	configuration("Debug")
		kind("ConsoleApp")
		targetdir("Ejercicio Aspiradora/Bin/Debug")
			--Linkeo con librerias del proyecto IA Core de la solucion y con las librerias de SFML
		links({"IA Core","sfml-main-d","sfml-system-s-d","sfml-window-s-d","sfml-graphics-s-d"})
			--Busco la direcion donde estan las librerias de SFML
		--libdirs({os.findlib("sfml-main-d")})
		 --IMPORTANTE EL PATH ES EL DE LIBRERIA DE SFML 1.6 CAMBIARLO AL QUE VALLA
		libdirs("D:/Libs/SFML/SFML-1.6/lib/vc2010")
	
	configuration("Release")
		kind("WindowedApp")
		targetdir("Ejercicio Aspiradora/Bin/Release")
			--Linkeo con librerias del proyecto IA Core de la solucion y con las librerias de SFML
		links({"IA Core","sfml-main","sfml-system-s","sfml-window-s","sfml-graphics-s"})
			--Busco la direcion donde estan las librerias de SFML
		--libdirs({os.findlib("sfml-main")})
		--IMPORTANTE EL PATH ES EL DE LIBRERIA DE SFML 1.6 CAMBIARLO AL QUE VALLA
		libdirs("D:/Libs/SFML/SFML-1.6/lib/vc2010")