'''mermaid



classDiagram

&nbsp;	ACharacter <|-- AKCharacterBase : Is-A

&nbsp;	AKCharacterBase <|-- AKPlayerCharacter : Is-A

&nbsp;	AKPlayerCharacter o-- UKInputConfig : References

&nbsp;	AKPlayerCharacter o-- UInputMappingContext : Uses

&nbsp;	

&nbsp;	class AKPlayerCharacter {

&nbsp;		#UKinputConfig\* InputConfig

&nbsp;		#UInputMappingContext\* DefaultIMC

&nbsp;		#SetupPlayerInputComponent(UInputComponent\* PlayerInputComponent)

&nbsp;		-InputMove(const FInputActionValue\& InValue)

&nbsp;		-InputLook(const FInputActionValue\& InValue)

&nbsp;	}

&nbsp;	

&nbsp;	class UKInputConfig {

&nbsp;		<<DataAsset>>

&nbsp;		+UInputAction\* Move

&nbsp;		+UInputAction\* Look

&nbsp;	}

&nbsp;	

&nbsp;	note for UKInputConfig "입력 액션(IA)들을 모아둔 에셋"

'''

