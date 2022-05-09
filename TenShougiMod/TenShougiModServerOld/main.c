/*
  BUG LIST                                                       

  - detection of repetitions can be wrong due to collision of hash keys and
    limitation of history table size.

  - detection of mates fails if all of pseudo-legal evasions are perpetual
    checks.  Father more, inferior evasions, such as unpromotion of
    bishop, rook, and lance at 8th rank, are not counted for the mate
    detection. 

  - detection of perpetual checks fails if one of those inferior
    evasions makes a position that occurred four times.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_WIN32)
#  include <fcntl.h>
#endif
#include "shogi.h"






//---------------------------------------------------------------------
#include "_for_launch.h"
// TenShougiMod.dllを起動するためのモード追加
//---------------------------------------------------------------------















int CONV_CDECL
#if defined(CSASHOGI)
main( int argc, char *argv[] )

#else
main()
#endif
{
  int iret;
  tree_t * restrict ptree;

#if defined(CSASHOGI)
//---------------------------------------------------------------------
  // 起動モードなら
  if ( _for_launch(argc, argv) ) {
	  // 関数内部でぐるぐる回っている。
	  // 関数から出てきたら終了
	  return 0;
  }
//---------------------------------------------------------------------
#endif

#if defined(TLP)
  ptree = tlp_atree_work;
#else
  ptree = &tree;
#endif

#if defined(CSASHOGI) && defined(_WIN32)
  FreeConsole();
  if ( argc != 2 || strcmp( argv[1], "csa_shogi" ) )
    {
      MessageBox( NULL,
		  "このプログラムから単体では起動できません。",
		  str_myname, MB_OK | MB_ICONINFORMATION );
      return EXIT_FAILURE;
    }
#endif

  if ( ini( ptree ) < 0 )
    {
      out_error( "%s", str_error );
      return EXIT_SUCCESS;
    }

  for ( ;; )
    {
#if defined(DEKUNOBOU)
      if ( dek_ngame && ( game_status & mask_game_end ) )
	{
	  TlpEnd();
	  if ( dek_next_game( ptree ) < 0 )
	    {
	      out_error( "%s", str_error );
	      break;
	    }
	}
#endif

      /* ponder a move */
      ponder_move = 0;
      iret = ponder( ptree );
      if ( iret == -1 )
	{
	  out_error( "%s", str_error );
	  ShutdownClient;
	  break;
	}
      else if ( iret == -2 )
	{
	  out_warning( "%s", str_error );
	  ShutdownClient;
	  continue;
	}
      else if ( game_status & flag_quit ) { break; }

      /* move prediction succeeded, pondering finished,
	 and computer made a move. */
      else if ( iret == 2 ) { continue; }

      /* move prediction failed, pondering aborted,
	 and we have opponent's move in input buffer. */
      else if ( ponder_move == MOVE_PONDER_FAILED )
	{
	}

      /* pondering is interrupted or ended.
	 do nothing until we get next input line. */
      else {
	TlpEnd();
	show_prompt();
      }

      iret = next_cmdline( 1 );
      if ( iret == -1 )
	{
	  out_error( "%s", str_error );
	  ShutdownClient;
	  break;
	}
      else if ( iret == -2 )
	{
	  out_warning( "%s", str_error );
	  ShutdownClient;
	  continue;
	}
      else if ( game_status & flag_quit ) { break; }

      iret = procedure( ptree );
      if ( iret == -1 )
	{
	  out_error( "%s", str_error );
	  ShutdownClient;
	  break;
	}
      if ( iret == -2 )
	{
	  out_warning( "%s", str_error );
	  ShutdownClient;
	  continue;
	}
      else if ( game_status & flag_quit ) { break; }
    }

  if ( fin() < 0 ) { out_error( "%s", str_error ); }

  return EXIT_SUCCESS;
}
